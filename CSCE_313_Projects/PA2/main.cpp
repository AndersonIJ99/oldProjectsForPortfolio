#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <cassert>
#include <assert.h>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <list>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <functional>
#include <cctype>
#include <locale>
#include <filesystem>
#include <ctime>
#include <limits.h>
#include <pwd.h>

using namespace std;

// gets data and time for custom prompt
// reference: https://pubs.opengroup.org/onlinepubs/007908775/xsh/time.h.html
string GetDateTime() {
    char buf[FILENAME_MAX];
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    std::string date_time(buf);
    return date_time;
}

// reference for split functions came initially from Geek for Geeks articles
// function for splitting strings delimited by spaces
vector<string> split(string line) {
    vector<string> ret;
    string word = "";
    for (auto x : line)
    {
        if (x == ' ')
        {
            ret.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    ret.push_back(word);
    return ret;
}

// function for splitting strings delimited by spaces and others chars
vector<string> splitWithDelim(string line, char delimiter) {
    vector<string> ret;
    string word = "";
    for (auto x : line)
    {
        if (x == delimiter)
        {
            ret.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    ret.push_back(word);
    return ret;
}

// convert vector of strings to array of chars
// reference: Tanzir's intro video 1 for PA2
char** vec_to_char_array(vector<string> commands) {
    char** ret = new char* [commands.size() + 1];
    for (int i = 0; i < commands.size(); i++) {
        // copy string into char array
        char* cstring = new char[commands[i].size() + 1];
        strcpy(cstring, commands[i].c_str());
        ret[i] = cstring;
    }
    ret[commands.size()] = NULL;

    return ret;
}

// reference: http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
// handle trimming of strings
std::string ltrim(std::string str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string rtrim(std::string str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string trim(std::string str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

// util function for removing quotes from a string
// reference: https://stackoverflow.com/questions/5674374/remove-double-quotes-from-a-string-in-c
std::string removeQuotes(std::string str) {
    std::string chars = "\'";
    for (char c: chars) {
        str.erase(std::remove(str.begin(), str.end(), c), str.end());
    }
    chars = "\"";
    for (char c: chars) {
        str.erase(std::remove(str.begin(), str.end(), c), str.end());
    }
    return str;
}

int main () {
    char cwd[PATH_MAX];

    vector<int> bgs;

    int sinb = dup(0);
    int soutb = dup(1);

    std::string cd = ""; // store the last cd command for cd -

    while (true) {
        for (int i = 0; i < bgs.size(); i++) {
            if (waitpid(bgs [i], 0, WNOHANG) == bgs [i]) {
                bgs.erase(bgs.begin() + i);
                i--;
            }
        }

        dup2 (sinb, 0);
        dup2 (soutb, 1);

        // Get the custom prompt (used stackoverflow)
//        time_t rawtime;
//        struct tm * timeinfo;
//        char timeday[80];
//        time (&rawtime);
//        timeinfo = localtime(&rawtime);
//        strftime(timeday, sizeof(timeday), "%d-%m-%Y %H:%M:%S", timeinfo);
//        std::string timestr(timeday);
//        char user[100];
//        FILE* name;
//        name = popen("whoami", "r");
//        fgets(user, sizeof(user-1), name);
//        std::string userstr(user);
//        pclose(name);
//        std::string prompt = userstr + " " + timestr + " $ ";

        // output custom prompt with user, date & time, and directory
        cerr << GetDateTime() << " : " << getpwuid(geteuid())->pw_name << ":~" << getcwd(cwd, sizeof(cwd)) << "$ ";

        // get user input
        string inputline;
        getline (cin, inputline); //get a line from standard input
        if (inputline == string("exit")) {
            cout << "Bye!! End of shell" << endl;
            break;
        }

        //initialize vector for piping parts
        vector<string> pparts;
        // find echo command if there is one
        int echoLoc = inputline.find("echo");
        // if there is no echo split by pipe char
        if (echoLoc < 0) {
            pparts = splitWithDelim(inputline, '|');
        }
        // else an echo exists so push back for now
        else { // if there is an echo
            pparts.push_back(inputline);
        }

        // parse through pipe parts
        for (int i = 0; i < pparts.size(); i++) {
            inputline = pparts[i]; // input is set to ith command to process
            inputline = trim(inputline); // trim off spaces
            int fds[2];
            pipe(fds);
            vector<string> parts; // initialize final vector for processed commands

            bool bg = false;
            // find bgs in input
            if (inputline[inputline.size() - 1] == '&') { // check for bgs
                //cout << "Bg process found" << endl;
                bg = true;
                inputline = trim(inputline.substr (0, inputline.size() - 1));
            }
            int pid = fork ();
            if (pid == 0){ //child process
                // find echo command in input
                if (trim(inputline).find("echo") == 0) {
                    // if found remove quotes
                    inputline = removeQuotes(inputline);
                    // then push back echo command and part to echo
                    parts.push_back("echo");
                    parts.push_back(trim(inputline.substr(5)));


                }
                else {
                    // print working directory execution
                    if (trim(inputline).find("pwd") == 0) {
                        char cwd[1024];
                        getcwd(cwd, sizeof(cwd));
                        printf("%s", cwd);
                    }
                    // changing directory execution
                    if (trim(inputline).find("cd") == 0) {
                        string dir = trim(splitWithDelim(inputline, ' ')[1]);
                        if (dir == "-") {
                            // if no previous dir print msg
                            if (cd == "") {
                                cerr << "No previous directory" << endl;
                            }
                            // else process change of directory
                            else {
                                chdir(cd.c_str());
                            }

                        }
                        else {
                            // use buffer to capture path to desired directory
                            char buff[FILENAME_MAX];
                            getcwd(buff, sizeof(buff));
                            cd = buff; // convert to string
                            chdir(dir.c_str());

                        }
                        continue;
                    }

                    // process redirection chars
                    int pos = inputline.find('>');
                    if (pos >= 0) {
                        // trim command and filename
                        string command = trim(inputline.substr(0, pos));
                        string filename = trim(inputline.substr(pos+1));
                        inputline = command;
                        // open file descriptor for >
                        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR);
                        dup2(fd, 1);
                        close(fd);
                    }
                    pos = inputline.find('<');
                    if (pos >= 0) {
                        // trim command and filename
                        string command = trim(inputline.substr(0, pos));
                        string filename = trim(inputline.substr(pos+1));
                        inputline = command;
                        // open file descriptor for <
                        int fd = open(filename.c_str(), O_RDONLY, S_IWUSR | S_IRUSR);
                        dup2(fd, 0);
                        close(fd); // Added
                    }

                    // find awk command
                    pos = inputline.find("awk");
                    if (pos >= 0) {
                        // trim and remove quotes from awk command args
                        inputline = removeQuotes(inputline);
                        inputline = trim(inputline); // remove spaces before and afters
                    }


                    if (i < pparts.size() -1) {
                        dup2(fds[1], 1);
                    }

                    // handle args for awk command
                    vector<string> initialParts = split(inputline);
                    int i = 0;
                    while (i < initialParts.size()) {
                        int dollarOp = -1;
                        // find dollar sign if not at the end of string
                        if (i != initialParts.size() - 1) {
                            dollarOp = initialParts.at(i+1).find("$");
                        }
                        // if dollar sign found parse into final parts vector
                        if (dollarOp >= 0) {
                            // skip parts of string by conjoining them
                            parts.push_back(initialParts.at(i) + " " + initialParts.at(i+1));
                            i += 2;
                        }
                        // else don't skip
                        else {
                            parts.push_back(initialParts.at(i));
                            i++;
                        }

                    }
                }
                // pass args to execvp
                char** args = vec_to_char_array(parts);
                execvp (args [0], args);

            }
            else {
                // if not at the end of pipe parts and not bg
                if ((i == pparts.size() - 1) && !bg) {
                    waitpid (pid, 0, 0); //parent waits for child process
                }
                else {
                    bgs.push_back (pid);
                }
                dup2(fds[0], 0);
                close(fds[1]);
            }
        }
    }
}


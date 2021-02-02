#include <iostream>
#include <fstream>
#include <unistd.h>
#include <wait.h>
#include <vector>
#include <sstream>
#include <limits.h>
#include <pwd.h>
#include <string>
#include <regex> // used for matching token input types
#include <algorithm>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

using namespace std;

// gets data and time for custom prompt
// reference: https://pubs.opengroup.org/onlinepubs/007908775/xsh/time.h.html
string GetDateTime(){
    char buf[FILENAME_MAX];
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    std::string date_time(buf);
    return date_time;
}

// reference: http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
// handle trimming of strings
string ltrim(string str, const string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

string rtrim(string str, const string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

string trim(string str, const string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

char** vec_to_char_array (vector<string>& parts) {
    char** result = new char * [parts.size() + 1]; // add 1 for the NULL character
    for(int i = 0; i < parts.size(); i++) {
        result[i] = (char*) parts[i].c_str();
    }
    result [parts.size()] = NULL;
    return result;
}

// function for splitting input string separated by spaces
// reference: Geeks for Geeks articles on string splitting
vector<string> split(string line) {
    /* Found part of this code from GeeksforGeeks */
    string word = "";
    vector<string> retvec;
    for (auto x : line)
    {
        if (x == ' ')
        {
            retvec.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    retvec.push_back(word);
    return retvec;
}

// function for splitting input string separated by a delimiter
// reference: Geeks for Geeks articles on string splitting
vector<string> split2(string line, char splitter) {
    /* Found part of this code from GeeksforGeeks */
    string word = "";
    vector<string> retvec;
    for (auto x : line)
    {
        if (x == splitter)
        {
            retvec.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    retvec.push_back(word);
    return retvec;
}

// function for splitting input string separated by a delimiter
// reference1: https://thispointer.com/how-to-split-a-string-in-c/
// reference2: https://stackoverflow.com/questions/7621727/split-a-string-into-words-by-multiple-delimiters
//vector<string> split (string line, string separator = " ") {
//    char delimiter = separator[0];
////    string search = separator;
////    if(separator != " ")
////        search = " " + separator;
//    stringstream ss(line);
//    string item;
//    vector<string> splitStrings;
////    while(getline(ss, item)) {
////        size_t prev = 0, pos;
////        while ((pos = item.find_first_of(search, prev)) != std::string::npos) {
////            if (pos > prev)
////                splitStrings.push_back(item.substr(prev, pos - prev));
////            prev = pos + 1;
////        }
////        if (prev < item.length())
////            splitStrings.push_back(item.substr(prev, std::string::npos));
////    }
//    while(getline(ss, item, delimiter))
//        splitStrings.push_back(item);
//    return splitStrings;
//}

// util function for removing quotes from a string
// reference: https://stackoverflow.com/questions/5674374/remove-double-quotes-from-a-string-in-c
string removequotes(string s) {
    // removing single quotes
    string chars = "\'";
    for (char c: chars) {
        s.erase(remove(s.begin(), s.end(), c), s.end());
    }
    // removing double quotes
    chars = "\"";
    for (char c: chars) {
        s.erase(remove(s.begin(), s.end(), c), s.end());
    }
    // return edited string
    return s;
}

int main() {
    char cwd[PATH_MAX];
    int sinb = dup(0);
    int soutb = dup(1);
    vector<int> bgs; // list of bgs
    string cd = ""; // stores the last change directory command for cd - case

    while(true) {
        for (int i = 0; i < bgs.size(); i++) {
            if(waitpid(bgs[i], 0, WNOHANG) == bgs[i]) {
                cerr << "Process: " << bgs[i] << " ended" << endl;
                bgs.erase(bgs.begin() + i);
                i--;
            }
        }

        dup2(sinb, 0);
        dup2(soutb, 1);

        // output custom prompt with user, date & time, and directory
        cerr << GetDateTime() << " : " << getpwuid(geteuid())->pw_name << ":~" << getcwd(cwd, sizeof(cwd)) << "$ ";

        // get user input
        string line;
        getline(cin, line); // get a line from standard input
        if(line == string("exit")) { // exiting shell
            cerr << "Bye!! End of shell" << endl;
            break;
        }

        //initialize vector for piping parts
        vector<string> pparts;
        // find echo command if there is one
        int echoLoc = line.find("echo");
        // if there is no echo split by pipe char
        if (echoLoc < 0) {
            pparts = split2(line, '|');
        }
        // else an echo exists so push back for now
        else {
            pparts.push_back(line);
        }

        // parse through pipe parts
        for(int i = 0; i < pparts.size(); i++) {
            line = pparts[i]; // input is set to ith command to process
            line = trim(line); // trims off spaces
            int fds[2];
            pipe(fds);
            vector<string> parts; // initialize final vector for processed commands

            bool bg = false;
            // find bgs in input
            if(line[line.size() - 1] == '&') { // check for bgs
                //cout << "Bg process found" << endl;
                bg = true;
                line = trim(line.substr(0, line.size() - 1));
            }
            int pid = fork();
            if(pid == 0) { //child process
                // find echo command in input
                if (trim(line).find("echo") == 0) {
                    // if found remove quotes
                    line = removequotes(line);
                    // then push back echo command and part to echo
                    parts.push_back("echo");
                    parts.push_back(trim(line.substr(5)));
                }
                else {
                    // print working directory execution
                    if (trim(line).find("pwd") == 0) {
                        char cwd[1024];
                        getcwd(cwd, sizeof(cwd));
                        printf("%s", cwd);
                    }
                    // changing directory execution
                    if (trim(line).find("cd") == 0) {
                        string dir = trim(split2(line, ' ')[1]);
                        if (dir == "-") {
                            // if no previous dir print msg
                            if (cd == "") {
                                cerr << "No previous directory" << endl;
                            }
                                // else process change of directory
                            else {
                                chdir(cd.c_str());
                            }
                        } else {
                            // use buffer to capture path to desired directory
                            char buff[FILENAME_MAX];
                            getcwd(buff, sizeof(buff));
                            cd = buff; // convert to string
                            chdir(dir.c_str());

                        }
                        continue;
                    }

                    // process redirection chars
                    int pos = line.find('>');
                    if (pos >= 0) {
                        line = trim(line);
                        string command = line.substr(0, pos);
                        string filename = line.substr(pos + 1);

                        line = command;
                        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR);
                        dup2(fd, 1);
                        close(fd);
                    }

                    pos = line.find('<');
                    if (pos >= 0) {
                        line = trim(line);
                        string command = line.substr(0, pos);
                        string filename = line.substr(pos + 1);

                        line = command;
                        int fd = open(filename.c_str(), O_RDONLY, S_IWUSR | S_IRUSR);
                        dup2(fd, 0);
                        close(fd);
                    }

                    // find awk command
                    pos = line.find("awk");
                    if (pos >= 0) {
                        // trim and remove quotes from awk command args
                        line = removequotes(line);
                        line = trim(line);
                    }

                    if (i < pparts.size() - 1) {
                        dup2(fds[1], 1);
                    }

                    // handle args for awk command
                    vector<string> initial_parts = split(line);
                    int i = 0;
                    while (i < initial_parts.size()) {
                        int dollarOp = -1;
                        // find dollar sign if not at the end of string
                        if (i != initial_parts.size() - 1) {
                            dollarOp = initial_parts.at(i+1).find("$");
                        }
                        // if dollar sign found parse into final parts vector
                        if (dollarOp >= 0) {
                            // skip parts of string by conjoining them
                            parts.push_back(initial_parts.at(i) + " " + initial_parts.at(i+1));
                            i += 2;
                        }
                        // else don't skip
                        else {
                            parts.push_back(initial_parts.at(i));
                            i++;
                        }
                    }
                }
                // pass args to execvp
                char **args = vec_to_char_array(parts);
                execvp(args[0], args);
            }
            else {
                // if not at the end pipe parts and not bg
                if((i == pparts.size() - 1) && !bg) {
                    waitpid(pid, 0, 0); // wait for the child process
                }
                else {
                    bgs.push_back(pid);
                }
                dup2(fds[0], 0);
                close(fds[1]);
            }
        }
    }
}


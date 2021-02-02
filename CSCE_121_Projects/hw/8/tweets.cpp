#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "functions.h"

/* Starter code: used by function printMenu */
void printLine(string msg) {
    cout << msg << endl;
}

/* Starter code: function printMenu() is ready for you */
void printMenu() {
    cout << endl;
    printLine("Welcome to Aggieland Twitter Feeds Stats");
    printLine("The options are:");
    printLine("1. load tweet data file and update stats");
    printLine("2. show overall stats (number of tweets, retweets, and hashtags)");
    printLine("3. show most popular hashtags");
    printLine("9. quit");
    cout << "--------> Enter your option: ";
}

/* Starter code: function to read option from the user is ready for you */
int getOption() {
    int option;
    cin >> option;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(256, '\n');
        return -1;
    } else {
	    cin.ignore(256, '\n');
    }
    return option;
}

int main() {
    string filename;
    int option;
    bool isRT;
    int numTweets = 0;
    int nb_htags = 0;
    int numRT = 0;
    string* arr_htags = nullptr;
    OrderedHashtagList hashlist;
    do {
        printMenu();
        option = getOption();
        if(option == 9) {
            break;
        }
        if((option < 9 && option > 3) || option > 9 || option < 0) {
            cout << "Invalid option" << endl;
            continue;
        }
        if(option == 1) {
            // stat reset for new files
            numTweets = 0;
            nb_htags = 0;
            numRT = 0;
            // get filename from user and check for proper open
            cout << "Enter filename: ";
            cin >> filename;
            ifstream ifs(filename);
            // check that file opens properly
            if (!ifs.is_open()) {
                cout << "File can't be open." << endl;
                continue;
            }
            // load tweet data and update stats
            while(!ifs.eof()) {
                int c;
                ifs >> c;
                if(!ifs.fail()) {
                    string line;
                    getline(ifs, line);
                    readTweet(line, isRT, nb_htags, arr_htags);
                    for(int i = 0; i < nb_htags; ++i) {
                        if(!insertHashtag(arr_htags[i], hashlist)) {
                            // reorder list in ascending order upon change in counters
                            Hashtag temp;
                            Hashtag temp2;
                            for (int i = 0; i < hashlist.size; ++i) {
                                for (int j = i + 1; j < hashlist.size; ++j) {
                                    if (hashlist.list[j].counter > hashlist.list[i].counter) {
                                        temp = hashlist.list[i];
                                        hashlist.list[i] = hashlist.list[j];
                                        hashlist.list[j] = temp;
                                        // working on insertion and shift logic
                                        // temp = hashlist.list[j];
                                        // for (int k = hashlist.size - 1; k > i; --k) {
                                        //     temp2 = hashlist.list[k];
                                        //     hashlist.list[k] = hashlist.list[k - 1];
                                        //     hashlist.list[k - 1] = temp2;
                                        // }
                                        // hashlist.list[i] = temp;
                                    }
                                }
                            }
                        }
                        cout << "run: " << endl;
                        showMostPopularHashtags(hashlist, 10);
                    }
                    delete [] arr_htags;
                    arr_htags = nullptr;
                    nb_htags = 0;
                    numTweets += 1;
                    if (isRT)
                        numRT += 1;
                }
            }
            continue;
        }
        if(option == 2) {
            cout << "Tweets: " << numTweets << ", Retweets: " << numRT << ", Hashtags: " << hashlist.size << endl;
            continue;
        }
        if(option == 3) {
            showMostPopularHashtags(hashlist, 10);
        }
    } while(option != 9);
    return 0;
}
   
    

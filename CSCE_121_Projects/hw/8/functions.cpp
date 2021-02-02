#include <iostream>
#include <string>
using namespace std;

#include "functions.h"



/* readTweet
 * Parameters:
 *      line: string containing the tweet information received by the function
 *      isRetweet: reference to bool; function will update it with true if retweet
 *      nb_htags: reference to int; function will update with number of hashtags
 *                in the tweet.
 *      array_of_htags: reference to an array of strings; function will allocate
 *                      the array and store the hashtag in it
 * Return value:
 *      none
 * Functionality:
 *     Processes the string in order to find out hashtags and identify if it is Retweet
 */
void readTweet(string line, bool& isRetweet, int& nb_htags, string*& array_of_htags) {
    // determine if tweet is retweet
    int foundRT = line.find("RT");
    if(foundRT != -1)
        isRetweet = true;
    else
        isRetweet = false;
    // finds hashtags in line and adds them to an array
    for(int i = 0; i < line.length(); ++i) {
        // found hashtag so add to array and increase array size
        if(line[i] == '#') {
            // goes from hashtag to next space
            //subs[i] = line.substr(i, line.find(' ', i));
            nb_htags += 1;
        }
    }
    array_of_htags = new string[nb_htags];
    int temp = 0;
    for(int i = 0; i < nb_htags; ++i) {
        int idx = line.find('#', temp);
        int htEnd = line.find(' ', idx) - idx;
        array_of_htags[i] = line.substr(idx, htEnd);
        temp = idx + htEnd;
    }
}

/* insertHashtag
 * Parameters:
 *      ht: string
 *      hashlist: reference to OrderedHashtagList struct
 * Return value:
 *      true if insertion succeeds, false if memory allocation
 *      fails.
 * Functionality:
 *      See prompt for specification and examples.
 */

bool insertHashtag(string ht, OrderedHashtagList& hashlist) {
    //bool inList = false;
    // lowercase the hashtag being sent
    for(int i = 1; i < ht.length(); ++i) {
        // only lowercase letters
        if(isalpha(ht[i]))
            ht[i] = tolower(ht[i]);
    }
    // attempt to insert hashtag
    if(hashlist.size != 0) {
        for(int i = 0; i < hashlist.size; ++i) {
            //cout << i << "\t" << hashlist.size << endl;
            // hashtag already in list
            if(hashlist.list[i].name == ht) {
                // increase counter and break out to reorder code
                hashlist.list[i].counter += 1;
                return false;
                //break;
            }
        }
    }
    // if(inList) {
        
    //     return false;
    // }
    // hashtag not in list so it gets inserted
    // resize if necessary
    if (hashlist.size == hashlist.capacity) {
        hashlist.capacity *= 2;
        Hashtag* temp = new Hashtag[hashlist.capacity];
        for(int i = 0; i < hashlist.size; ++i) {
            temp[i] = hashlist.list[i];
        }
        delete [] hashlist.list;
        hashlist.list = temp;
    }
    // create hashtag, make room (increase size), and insert
    Hashtag htag = {ht, 1};
    // cout << htag.name << endl;
    // cout << hashlist.size << endl;
    hashlist.list[hashlist.size] = htag;
    hashlist.size += 1;
    return true;
}

/* showMostPopularHashtags
 * Parameters:
 *      hashlist: reference to OrderedHashtagList struct
 * Return value:
 *      none
 * Functionality:
 *      Prints the k most popular hashtags.
 *      See prompt for formatting details.
 */
void showMostPopularHashtags(OrderedHashtagList hashlist, int k) {
    if(hashlist.size == 0)
        cout << "No hashtags." << endl;
    if(k > hashlist.size)
        k = hashlist.size;
    for(int i = 0; i < k; ++i) {
        cout << "Tag " << hashlist.list[i].name << " - " << hashlist.list[i].counter << " occurrence(s)" << endl;
    }
}



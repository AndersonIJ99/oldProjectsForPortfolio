/**************************************************************
 * File:    driver.cpp
 * Project: CMSC 341 - Project 3 - Word Frequency
 * Author : 
 * Date   : 04-November-2014
 * Section: Lecture-02
 * E-mail:  
 *
 * HashedSplays Class implementation.
 *
 *************************************************************/
#include "HashedSplays.h"  // Includes constants

using namespace std;

int main(int argc, char *argv[]) {

    int ALPHABET_SIZE = 26;
    try {
        // Instatiate the main object
        HashedSplays wordFrequency(ALPHABET_SIZE);
        // Build the trees
        wordFrequency.FileReader(argv[1]);

        // Test methods to show hashed splay trees work
        wordFrequency.PrintHashCountResults();
        wordFrequency.PrintTree(19); // Prints the "T" tree
        wordFrequency.PrintTree("F");
        wordFrequency.PrintTree("K"); // should be mpty running input1
        wordFrequency.FindAll("The"); // should find all the's (ignoring case)
        cout << endl;
    }
    // Error catching
    catch (Exceptions &cException) {
        cout << "EXCEPTION: " << cException.GetMessage() << endl;
        return 1;
    }
    
    //Uncomment for number of seconds running.
    //cout << clock() / (float)(CLOCKS_PER_SEC) << endl;
    return 0;
}

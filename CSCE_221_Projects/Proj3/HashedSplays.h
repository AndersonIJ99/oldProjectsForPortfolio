/*****************************************
** File:    HashedSplays.h
** Project: CSCE 221 Project 3
** Author:  Iain Anderson
** Date: 3/30/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Header file laying out the Hashed Splays class.
**
*******************************************/

#ifndef HASHED_SPLAYS_H
#define HASHED_SPLAYS_H

#include "dsexceptions.h"
#include "Exceptions.h"
#include "SplayTree.h"
#include "Node.h"
#include "Util.h"
#include <iostream> // For NULL
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class HashedSplays
{
public:
    // Constructor:  sets up table to properly hold SplayTrees and their Nodes
    HashedSplays(int size) : m_trees(size) {
        table.resize(size);

        for(int i = 0; i < size; i++)
            table.at(i) = SplayTree<Node>();
        cout << "Building hashed splay..." << endl;
    }
    
    // Deconstructor: Clears table after making its SplayTrees empty
    ~HashedSplays() {
        // reset m_trees
        m_trees = 0;
        // clear SplayTrees in table
        for(unsigned int i = 0; i < table.size(); i++)
            table.at(i).makeEmpty();
        // clear the vector's data
        table.clear();
    }

    // Reads in word by word from a file to be put into table
    void FileReader(string inFileName) {
        // Instantiate necessary variables
        ifstream inFile;
        string input;
        Node n;
        string word;
        int wordCount = 0;

        // Check for file
        inFile.open(inFileName);
        if(!inFile)
            cout << "File not found." << endl;
        else {
            
            while (inFile >> input)
            {
                // Strip input to get word to be inserted
                word = Util::Strip(input);
                // Check to see if it is a word
                if(word.length() == 0)
                    continue;
                
                wordCount += 1;

                // Create Node for word to be used when inserting
                n = Node(word, 1);
                // solve for location of word in table
                int position = GetIndex(word);

                // Check to see if the word is already in the table
                if(!table.at(position).contains(n)) {
                    // if not, then add it and increment node count (within insert function)
                    table.at(position).insertBootstrap(n);
                }
                else {
                    // was found, so increment frequency
                    table.at(position).increaseFrequency();
                }
            }
        }
    }

    // Prints SplayTree given a "letter"
    // Will call the index version to do the real work
    void PrintTree(string letter) {
        int position = GetIndex(letter);
        PrintTree(position);
    }

    // Prints SplayTree given an index
    void PrintTree(int index) {
        // Avoid unsigned integer comparison by casting as new integer variable
        int tableSize = table.size();
        // Out of bounds check
        if(index < 0 || index > tableSize) {
            throw ArrayIndexOutOfBounds();
            return;
        }
        // Output the SplayTree and the number of times it has splayed
        table.at(index).printTree();
        cout << "\nThe tree has had " << table.at(index).getNumSplays() << " splays." << endl;
    }

    // Iterate through table and display its data
    void PrintHashCountResults() {
        // iterate through table and print the root of each SplayTree
        for(unsigned int i = 0; i < table.size(); i++) {
            // Check for an empty SplayTree
            if(table.at(i).isEmpty())
                cout << "This tree has no nodes" << endl;
            else {
                cout << "This tree starts with ";
                // printRoot handles display of root node
                table.at(i).printRoot();
                int nodeCount = table.at(i).getNumNodes();
                cout << " and has " << nodeCount << " nodes" << endl;
            }
        }
    }

    // Finds and displays all strings containing the inPart parameter
    // This version calls the bootstrap located in the SplayTree class
    void FindAll(string inPart) {
        int position = GetIndex(inPart);
        cout << "Printing the results of nodes that start with \'" << inPart << "\'" << endl;
        // Find all upper cased strings
        inPart.at(0) = toupper(inPart.at(0));
        table.at(position).FindAll(inPart);
        // Find all lower case strings
        inPart.at(0) = tolower(inPart.at(0));
        table.at(position).FindAll(inPart);
    }

    // Calculates the index of a word in table
    // Based on alphanumeric order
    int GetIndex(string inLetter) {
        // Illegal argument check
        if(inLetter.length() < 1) {
            throw IllegalArgumentException();
            exit(1);
        }
        return Util::Lower(inLetter).at(0) - 'a';
    }
private:
    vector<SplayTree<Node>> table;
    int m_trees;
};

#endif

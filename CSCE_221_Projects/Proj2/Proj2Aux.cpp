/*****************************************
** File:    Proj2Aux.cpp
** Project: CSCE 221 Project 2
** Author:  Iain Anderson
** Date:    2/23/2020
** Section: 518
** E-mail:  iain@tamu.edu
**
**  C++ file:
**	This file defines the functions from its
**  header file to be called in a driver file.
**
*******************************************/

using namespace std;

#ifndef PROJ_2_AUX_C
#define PROJ_2_AUX_C

/*---------------Imported Libraries---------------*/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

/*----------------Includes---------------------*/
#include "Proj2Aux.h"

Proj2Aux::Proj2Aux() {}

Proj2Aux::~Proj2Aux() {
    m_tree.makeEmpty();
}

int Proj2Aux::ReadFromCommandFile(char *commandFile) {
    inFile.open(commandFile);
    if (!inFile.is_open()) {
        // cout << "Unable to open file: " << commandFile << endl;
        return 0; // Failure to open file so return "false"
    }

    // Read line by line & delimited by spaces
    char comment = '#';
    string line = "";
    string command = "";
    
    // Continue while getting each line
    while (getline(inFile, line)) {
        istringstream ss(line);

        // Get first word
        ss >> command;

        // If the line is a comment or NULL
        if (line.at(0) == comment || line == "") {
            string line = "";
            string command = "";
            continue;
        }
        
        // Command value integer
        int n;
        // if can put int from the stream
        if (ss >> n) {
            if (command == "NTH") {
                cout << "NTH " << n << ": ";
                cout << GetNthElementCommand(command, n) << endl;
                cout << endl;
            }
            else if (command == "PRINT") {
                cout << "PRINT " << n << ":" << endl;
                DoPrintCommand(command, n);
            }
            else if (command == "REMOVE") {
                cout <<"REMOVE " << n << ": ";
                int remove = DoRemoveCommand(command, n);
                cout << ((remove > 0) ? "SUCCESS" : "FAILED") << endl;
                cout << endl;
            }
            else if (command == "RANK") {
                cout << "RANK " << n << ": ";
                cout << GetRankCommand(command, n) << endl;
                cout << endl;
            }
            else {
                cerr << "Invalid Command Exception: " << command <<  " is not a valid command" << endl;
            }
        }
        // If there is no number to get after the command it must fall here
        else {
            if (command == "MEDIAN") {
                cout << "MEDIAN: ";
                cout << GetMedianCommand(command) << endl;
                cout << endl;
            }
            else if (command == "PERFECT") {
                cout << "PERFECT: ";
                int perfect = IsPerfect(command);
                cout << ((perfect > 0) ? "YES" : "NO") << endl;
                cout << endl;
            }
            else if (command == "RESIDUALS") {
                cout << "\nRESIDUALS: ";
                cout << GetResidualsCommand(command) << endl;
                cout << endl;
            }
            else if (command == "COMPLETE") {
                cout << "\nRESIDUALS: ";
                cout << "Extra credit command not implemented." << endl;
                cout << endl;
            }
            else {
                cerr << "Invalid Command Exception: " << command <<  " is not a valid command" << endl;
            }
        }
        // reset line and command values
        line = "";
        command = "";
        
    }

    inFile.close();

    return 1; // Successfully read file so return "true"
}

// Read in from input files
int Proj2Aux::ReadFromInputFile(char *inputFile) {
    inFile.open(inputFile);
    if (!inFile.is_open()) {
        // cout << "Unable to open file: " << inputFile << endl;
        return 0; // Failure to open file so return "false"
    }

    // https://stackoverflow.com/questions/14516915/read-numeric-data-from-a-text-file-in-c/14517130
    int value;
    while(inFile >> value) {
        m_tree.insert(value);
    }

    inFile.close();

    return 1; // Successfully read file so return "true"
}

void Proj2Aux::DoPrintCommand(string command, int depth) {
    m_tree.PrintLevels(depth);
}

int Proj2Aux::DoRemoveCommand(string command, int element) {
    return m_tree.remove(element);
}

int Proj2Aux::GetMedianCommand(string command) {
        return m_tree.Median();
}

int Proj2Aux::GetNthElementCommand(string command, int nthElement) {
    return m_tree.NthElement(nthElement);
}

int Proj2Aux::GetRankCommand(string command, int element) {
    return m_tree.Rank(element);
}

int Proj2Aux::GetResidualsCommand(string command) {
    return m_tree.RemoveResidue();
}

// bool Proj2Aux::IsComplete(string command) {}

bool Proj2Aux::IsPerfect(string command) {
    return m_tree.IsPerfect();
}

#endif
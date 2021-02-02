/*****************************************
** File:    Proj2Aux.h
** Project: CSCE 221 Project 2
** Author:  Iain Anderson
** Date:    2/23/2020
** Section: 518
** E-mail:  iain@tamu.edu
**
**  C++ file:
**	This header file lays functions necessary to
**  read in inputs into the BST so that the augments
**  may be tested.
**
*******************************************/

using namespace std;

#ifndef PROJ_2_AUX_H
#define PROJ_2_AUX_H

/*---------------Imported Libraries---------------*/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

/*----------------Includes---------------------*/
#include "AugmentedBinarySearchTree.h"

class Proj2Aux {
    public:
        Proj2Aux();
        ~Proj2Aux();
        int ReadFromCommandFile(char* commandFile);
        int ReadFromInputFile(char* inputFile);
        void DoPrintCommand(string command, int depth);
        int DoRemoveCommand(string command, int element);
        int GetMedianCommand(string command);
        int GetNthElementCommand(string command, int nthElement);
        int GetRankCommand(string command, int element);
        int GetResidualsCommand(string command);
        // bool IsComplete(string command);
        bool IsPerfect(string command);

    private:
        ifstream inFile;
        AugmentedBinarySearchTree<int> m_tree;
};

#endif
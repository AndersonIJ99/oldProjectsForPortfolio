using namespace std;

#ifndef PROJ_3_AUX_H
#define PROJ_3_AUX_H

/*---------------Imported Libraries---------------*/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

/*----------------Includes---------------------*/
#include "AugmentedBinarySearchTree.h"

class Proj3Aux {
    public:
        Proj3Aux();
        ~Proj3Aux();
        int ReadFromCommandFile(char* commandFile);
        int ReadFromInputFile(char* inputFile);
        void DoPrintCommand(string command, int depth);
        int DoRemoveCommand(string command, int element);
        int GetMedianCommand(string command);
        int GetNthElementCommand(string command, int nthElement);
        int GetRankCommand(string command, int element);
        int GetResidualsCommand(string command);
        bool IsComlete(string command);
        bool IsPerfect(string command);
    private:
        ifstream inFile;
        AugmentedBinarySearchTree<int> m_tree;
}

#endif
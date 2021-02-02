/**************************************************************
 * File:    Util.cpp
 * Project: CMSC 341 - Project 3 - Word Frequency
 * Author : 
 * Date   : 04-November-2014
 * Section: Lecture-02
 * E-mail:  
 *
 * Util Class implementation.
 *
 * This class provides some support functions to other classes.
 * 
 * Lower() - Returns a string in lower case.
 * Strip() - Removes all nonalpha characters except ' and -
 * FileExisits() - Verifes a file exists.
 * 
 *************************************************************/
#include "Util.h"

using namespace std;


//Retuns the string lower cased.
string Util::Lower(string inString)
{
    for (unsigned int i = 0; i < inString.length(); i++) {
        inString[i] = tolower(inString[i]);
    }
    return inString;
}


//Returns the string with only alpha characters.
string Util::Strip(string inString)
{
    std::string newString;
    
    if (inString == "") return inString;
    
    else if (!isalpha(inString[0])) {
        for (unsigned int i = 1; i < inString.length(); i++) {
            newString += inString[i];
        }
        return Strip(newString);
    }
    
    else if (!isalpha(inString[inString.length() - 1])) {
        for (unsigned int i = 0; i < inString.length() - 1; i++) {
            newString += inString[i];
        }
        return Strip(newString);
    }
    
    bool contraction = false;
    bool foundalpha = 0;
    
    for (unsigned int i = 0; i < inString.length(); i++)
    {
        if (isalpha(inString[i])) {
            newString += inString[i];
            foundalpha = true;
        } else if ((int(inString[i]) == 39 || int(inString[i]) == 45) && 
            !contraction && foundalpha) {
            newString += inString[i];
            contraction = true;
        }
    }
    
    
    return newString;
    
}


bool Util::FileExists(const char* filename)
{
    ifstream inFile(filename);
    
    if (!inFile) {
        return false;
    }
    else {
        inFile.close();
        return true;
    }
}
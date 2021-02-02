/**************************************************************
 * File:    Node.h
 * Project: CMSC 341 - Project 3 - Word Frequency
 * Author : 
 * Date   : 04-November-2014
 * Section: Lecture-02
 * E-mail:  
 *
 * Node Class definition.
 *************************************************************/
#ifndef NODE_H
#define NODE_H

#include "Util.h" // For some string functions

using namespace std;

class Node{

public:
    /**********************************************************************
     * Name: Node (Constructor)
     * PreCondition: None.  Non parameter constructor requried for
     * container storage  
     * 
     * PostCondition:  Empty node object.
     *********************************************************************/
    Node();
    
    
    /**********************************************************************
     * Name: Node (Constructor)
     * PreCondition: Word (string) and frequency for bulding a node
     * 
     * PostCondition:  Node with word and frequency
     *********************************************************************/
    Node(string inWord, int frequency);
    
    
    /**********************************************************************
     * Name: Node (Destructor)
     * PreCondition: None
     * 
     * PostCondition:  None
     *********************************************************************/
    ~Node();
    
    
    /**********************************************************************
     * Name: GetWord
     * PreCondition: None
     * 
     * PostCondition:  String value of the word
     *********************************************************************/
    string GetWord() const;
    
    
    /**********************************************************************
     * Name: GetFrequency()
     * PreCondition: None
     * 
     * PostCondition:  Int value of the frequency
     *********************************************************************/
    int GetFrequency() const;
    
    
    /**********************************************************************
     * Name: IncrementFrequency
     * PreCondition: None
     * 
     * PostCondition:  Increments the word's frequency by 1
     *********************************************************************/
    void IncrementFrequency();

    
    /**********************************************************************
     * Name: Overload operator <
     * PreCondition: Valid node objects to compare
     * 
     * PostCondition:  Bool.  True if the word value is less.
     *********************************************************************/
    bool operator<(const Node &RHS) const;
    
    
    /**********************************************************************
     * Name: Overload operator ==
     * PreCondition: Valid node objects to compare
     * 
     * PostCondition:  Bool.  True if the word values are equal.
     *********************************************************************/
    bool operator==(const Node &RHS);
    
    
    /**********************************************************************
     * Name: Overload operator =
     * PreCondition: Valid node objects to compare
     * 
     * PostCondition:  New node via a deep copy.
     *********************************************************************/
    Node operator=(const Node &RHS);
    
    
    /**********************************************************************
     * Name: Overload operator %
     * PreCondition: Valid node objects to compare
     * 
     * PostCondition:  Bool.  True if the word in this object is a 
     * leading SUBSTRING of the compared node.
     *********************************************************************/
    bool operator%(const Node& RHS) const;

    
    /**********************************************************************
     * Name: Overload operator << (Friend)
     * PreCondition: Valid node to output
     * 
     * PostCondition:  Formated output of the word and frequency.
     *********************************************************************/
    friend std::ostream& operator<<(std::ostream& out, const Node &inNode);
    
private:
    std::string m_word; // The word
    int m_frequency;    // How often the word has appeared.
};

#endif

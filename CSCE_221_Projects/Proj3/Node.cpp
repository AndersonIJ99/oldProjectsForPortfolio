/**************************************************************
 * File:    Node.cpp
 * Project: CMSC 341 - Project 3 - Word Frequency
 * Author : 
 * Date   : 04-November-2014
 * Section: Lecture-02
 * E-mail:  
 *
 * Node Class implementation.
 *************************************************************/
#include "Node.h"

#include <iostream>

using namespace std;

//No parameter constructor for containers
Node::Node(){}


//Full constructor
Node::Node(string inWord, int frequency) : m_word(inWord),
    m_frequency(frequency){}

    
//Destructor
Node::~Node(){}


//Compares this to RHS and returns true if the word is less than
bool Node::operator<(const Node& RHS) const
{
    return (this->m_word < RHS.m_word);
}


//Compares this to RHS and returns true if the words are identical
bool Node::operator==(const Node& RHS)
{
    return (this->m_word == RHS.m_word);
}


//Deep copy
Node Node::operator=(const Node& RHS)
{
    //Be sure we aren't copying over the node
    if( this != &RHS )
    {
        this->m_word = RHS.m_word;
        this->m_frequency = RHS.m_frequency;
    }
    return *this;
}


//Check to see if we have a substring
bool Node::operator%(const Node& RHS) const
{
    //We want to ignore case on this check
    string text = this->GetWord();
    text = Util::Lower(text);
    
    string compared = RHS.GetWord();
    compared = Util::Lower(compared);
    
    // If the substring is longer it really isn't a substring
    if (text.length() > compared.length()) {return false;}
    //Check each character of the substring against the compared string
    for (unsigned int i = 0; i < text.length(); i++){
        if (text[i] != compared[i]){
            //One miss is all it takes to not have a match
            return false;
        }
    }
    return true;
}


//Increment the frequency
void Node::IncrementFrequency()
{
    m_frequency++;
}


//Formatted output
std::ostream& operator<<(std::ostream& out, const Node &inNode)
{
    out << "Node [word=" << inNode.GetWord() << ", frequency=";
    out << inNode.GetFrequency() << "]" ;
    return out;
}


//Return the int for frequency
int Node::GetFrequency() const
{
    return m_frequency;
}


//Return the string for the word
string Node::GetWord() const
{
    return m_word;
}
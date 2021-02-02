/********************************************************************
** File     :  Llama.cpp
** Project  :  CSCE 221 Project 1, Spring 2020
** Author   :  Iain Anderson
** Date     :  2/3/20
** Section  :  518
** E-mail   :  iain@tamu.edu
**
**  This file implements the functions that are declared in Llama.h.
**
*********************************************************************/

#ifndef _LLAMA_CPP_
#define _LLAMA_CPP_

#include <iostream>
#include "Llama.h"
using namespace std;

// Default Constructor for Llama
// Sets FRONT and REAR ptr to a newly created initial node
template <class T, int LN_SIZE>
Llama<T, LN_SIZE>::Llama() {
    FRONT_ptr = NULL;
    REAR_ptr = NULL;
    LlamaNode<T, LN_SIZE> *initialNode = new LlamaNode<T, LN_SIZE>;
    FRONT_ptr = initialNode;
    REAR_ptr = initialNode;
}

// Copy constructor for Llama
template <class T, int LN_SIZE>
Llama<T, LN_SIZE>::Llama(const Llama<T, LN_SIZE> &other) {
    copy();
}

// Destructor for Llama
template <class T, int LN_SIZE>
Llama<T, LN_SIZE>::~Llama() {
    clear();
}

// Returns the number of items in the Llama Stack
template <class T, int LN_SIZE>
int Llama<T, LN_SIZE>::size() {
    int numItems = 0;

    //Iterates through nodes in Llama Stack
    for (CURSOR = FRONT_ptr; CURSOR; CURSOR = CURSOR->link) {
        // Iterates through the items in a Llama Node's array
        for(int iter = 0; iter < CURSOR->arr.size(); ++iter) {
            numItems++;
        }
    }
    
    // Safely handles CURSOR
    delete CURSOR;
    CURSOR = NULL;

    return numItems;
}

// Prints out all pertinent information for this Llama Stack
template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::dump() {
    for (CURSOR = FRONT_ptr; CURSOR; CURSOR = CURSOR->link) {
        // Iterates through the items in a Llama Node's array
        cerr << CURSOR;
    }
    // Outputs the number of nodes created/destroyed
    CURSOR->report();
    if(this->extra) {
        cout << "There is an extra node!" << endl;
        cerr << extraCount << " Elements remaining before deletion of current extra node" << endl;
    }
    else
        cout << "No extra node detected." << endl;
}

// Adds data to the top of the Llama Stack
template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::push(const T &data) {
    // check for an extra node and if the node previous extra node is full
    // maybe have check that sets !extra if node previous extra node is full
    // if extra then get to the node before REAR to push
    // else check if REAR is full
    // if true create new node and push
    // else push onto REAR's array
}

// Removes and returns the item at the top of the Llama Stack
// template <class T, int LN_SIZE>
// T Llama<T, LN_SIZE>::pop() {
    // check for extra node
    // if true get to node previous of extra node to find top value to pop
    // else pop top value of REAR's array
// }

// Duplicates the top of the Llama Stack
// template <class T, int LN_SIZE>
// void Llama<T, LN_SIZE>::dup(); //  (top) A B C D -> A A B C D
// create a new node to be inserted at the front of the Llama Stack
// Base off of push_front functions from Linked List

// Swaps the top two items of the Llama Stack
// template <class T, int LN_SIZE>
// void Llama<T, LN_SIZE>::swap(); //  (top) A B C D -> B A C D
// attempt to use temp variables to hold m_next values from first two nodes so
// that list can be reconnected after swap

// Permutes the top three items of the Llama Stack
// template <class T, int LN_SIZE>
// void Llama<T, LN_SIZE>::rot(); //  (top) A B C D -> C A B D
// attempt to use temp variables to hold m_next values from first two nodes so
// that list can be reconnected after swap
// might get complex since it's not first two nodes

// Returns the value of an item offset from the top of the Llama Stack
// template <class T, int LN_SIZE>
// T Llama<T, LN_SIZE>::peek(int offset) const;
// offset can go into lower nodes from the top
// check to see if offset is greater than size()
// if true throw exception
// else check to see if extra node
// if true then check to see if offset > extraCount
// go until node where index is


// overloaded assignment operator
template <class T, int LN_SIZE>
const Llama<T, LN_SIZE>& Llama<T, LN_SIZE>::operator=(const Llama<T, LN_SIZE> &rhs) {
    // Checks if two Stacks are the same and handles a difference in size
    if(this != &rhs) {
        clear();
        copy(&rhs);
    }
    return *this;
}

// To be called by copy constructor or assignment operator to perform a copy of source Stack onto this Stack
template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::copy(const Llama<T, LN_SIZE> &other) {
    //Iterates through nodes in Llama Stack
    for (CURSOR = other->FRONT_ptr; CURSOR; CURSOR = CURSOR->m_next) {
        // Pushes items from the current node's array onto this node's array
        for(int iter = 0; iter < CURSOR->arr.size(); ++iter) {
            // make push function with resizability
            // this->push(CURSOR->arr[iter]);
        }
    }
}

// Called by the deconstructor to delete the Llama Stack
template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::clear() {
    // Deletes Llama Stack from the front forward
    while (FRONT_ptr != NULL) {
        CURSOR = FRONT_ptr;
        FRONT_ptr = FRONT_ptr->m_next;
        delete CURSOR;
    }
    // Safely handles FRONT, REAR, and CURSOR
    FRONT_ptr = NULL;
    REAR_ptr = NULL;
    CURSOR = NULL;
}

#endif
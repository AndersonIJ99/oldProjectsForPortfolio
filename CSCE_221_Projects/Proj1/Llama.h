/********************************************************************
** File     :  Llama.h
** Project  :  CSCE 221 Project 1, Spring 2020
** Author   :  Iain Anderson
** Date     :  2/3/20
** Section  :  518
** E-mail   :  iain@tamu.edu
**
**  This file declares the functions necessary to create and
**  manipulate a linked list of arrays (Llama).
**
*********************************************************************/

#ifndef _LLAMA_H_
#define _LLAMA_H_

/* File: Llama.h

   This file has the class declaration for the LlamaNode class
   for Project 1. See project description for details.

   You may add public and private data members to the Llama class.

   You may add public and private member functions to the Llama class.

*/


#include <stdexcept>
#include <string>
#include "LlamaNode.h"

using namespace std ;


class LlamaUnderflow : public std::out_of_range {

   public:

   LlamaUnderflow(const string& what) : std::out_of_range(what) { }

} ;


template <class T, int LN_SIZE>
class Llama {

   public:

   Llama() ;
   Llama(const Llama<T,LN_SIZE>& other) ;   // copy constructor
   ~Llama() ;


   int size() ; 
   void dump() ;
   void push(const T& data) ;
   // T pop() ;


   // void dup() ;    //  (top) A B C D -> A A B C D
   // void swap() ;   //  (top) A B C D -> B A C D 
   // void rot() ;    //  (top) A B C D -> C A B D


   // T peek(int offset) const ;


   // overloaded assignment operator
   //
   const Llama<T,LN_SIZE>& operator=(const Llama<T,LN_SIZE>& rhs) ;


   //
   // Add your public member functions & public data mebers here:
   //


   private:

   bool extra;
   int extraCount;
   LlamaNode<T, LN_SIZE> *FRONT_ptr, *REAR_ptr, *CURSOR, *INSERT;

   void copy(const Llama<T,LN_SIZE>& other);
   void clear();


} ;


#include "Llama.cpp"


#endif

/********************************************************************
** File     :  LlamaNode.h
** Project  :  CSCE 221 Project 1, Spring 2020
** Author   :  Iain Anderson
** Date     :  2/3/20
** Section  :  518
** E-mail   :  iain@tamu.edu
**
**  This file lays out a templated class for the nodes of Llama with its private
**  and public members.
**
*********************************************************************/

#ifndef _LLAMANODE_H_
#define _LLAMANODE_H_


/* File: LlamaNode.h

   This file has the class declaration for the LlamaNode class
   for Project 1. See project description for details.

   This file should not be modified in ANY way.

   When your program is graded, it will be compiled with the original 
   version of this file. Your program must work with the original. 

*/


#include <iostream>
using namespace std ;


template <class T, int LN_SIZE>   // forward class declaration
class Llama ;


template <class T, int LN_SIZE>
class LlamaNode {

   friend class Llama<T,LN_SIZE> ;

   public:

   LlamaNode()  ;
   ~LlamaNode() ;

   static void report() ; 


   private:   

   static int newCount ;      // # of times constructor was called
   static int deleteCount ;   // # of times desctructor was called

   T arr[LN_SIZE] ;
   LlamaNode *m_next ;   

} ;


#include "LlamaNode.cpp"

#endif
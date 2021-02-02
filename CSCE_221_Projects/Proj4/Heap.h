/*****************************************
** File:    Heap.h
** Project: CSCE 221 Project 4
** Author:  Iain Anderson
** Date: 4/28/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Header for the base Heap class.
**
*******************************************/

#ifndef HEAP_H_
#define HEAP_H_

#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

template<class T, int m_size> class Heap{

	public:

		// required functions by UML
		Heap();
		Heap(const Heap<T, m_size>& origHeap);
		bool Contains(const T& needle) const;	
		const T* Find(const T& needle) const;
		// Virtual function will never be used as base class will depend
		// on its children to do the work
		virtual T& Remove() { return m_array[1]; }
		void Insert(T& insertable);		
		void PercolateUp(int index);
		void PercolateDown(int index);

		// student-made functions below
		void buildHeap();
		Heap(const vector<T> & items);
		T& getArrayIndexValue(int index);
		int getArraySize();
		void resizeArray();

	private:

		// UML-required member variables
		T* m_array;

		// student-made member variables
		int currentSize = m_size;
};

#include "Heap.cpp"
#endif /* HEAP_H_ */

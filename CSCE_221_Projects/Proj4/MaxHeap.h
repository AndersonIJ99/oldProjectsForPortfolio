/*****************************************
** File:    MaxHeap.h
** Project: CSCE 221 Project 4
** Author:  Iain Anderson
** Date: 4/28/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Header for the MaxHeap class.
**
*******************************************/

#ifndef MAX_HEAP_H_
#define MAX_HEAP_H_

#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

#include "Heap.h"

template<class T, int m_size> class MaxHeap : public Heap<T, m_size> {

	public:

        MaxHeap(const Heap<T, m_size> &origHeap);
        bool Contains(const T& needle) const;	
		const T* Find(const T& needle) const;
		T& Remove();	
		void Insert(T& insertable);		
		void PercolateUp(int index, T& insertable);
		void PercolateDown(int index);

		// student-made functions below
		void buildHeap();

	private:

		// student-made member variables
		Heap<T, m_size> *heap;
		int currentSize = m_size;
};

#include "MaxHeap.cpp"
#endif /* MAX_HEAP_H_ */

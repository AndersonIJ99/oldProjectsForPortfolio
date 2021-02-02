/*****************************************
** File:    Heap.cpp
** Project: CSCE 221 Project 4
** Author:  Iain Anderson
** Date: 4/28/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Define the functions laid out in its header.
**
*******************************************/

#ifndef HEAP_CPP_
#define HEAP_CPP_
#include "Heap.h"



	template<class T, int m_size>
	Heap<T, m_size>::Heap() {
		currentSize = 0;
	}

	template<class T, int m_size>
	Heap<T, m_size>::Heap(const Heap<T, m_size>& origHeap) {
		m_array = new T[m_size];
		for(int i = 1; i < m_size; ++i)
			m_array[i] = origHeap.m_array[i];
	}

	template<class T, int m_size>
	bool Heap<T, m_size>::Contains(const T& needle) const {
		if(currentSize == 0) {
			// An empty heap contains nothing
			return false;
		}

		for(int i = 0; i < currentSize; i++) {
			if(needle == m_array[i])
				return true;
		}
		return false;
	}	


	template<class T, int m_size>
	const T* Heap<T, m_size>::Find(const T& needle) const {
		T *found;

		if (!this->Contains(needle)) {
			std::cerr << "Item not in heap." << std::endl;
			exit(1);
		}

		for (int i = 0; i < currentSize; i++) {
			if (needle == m_array[i]) {
				found = &m_array[i];
				break;
			}
		}
		return found;
	}

	template<class T, int m_size>
	void Heap<T, m_size>::Insert(T& insertable) {

	}	

	template<class T, int m_size>
	void Heap<T, m_size>::PercolateUp(int index) {

	}

	template<class T, int m_size>
	void Heap<T, m_size>::PercolateDown(int index) {

	}

	template <class T, int m_size>
	void Heap<T, m_size>::buildHeap() {

	}

	template <class T, int m_size>
	Heap<T, m_size>::Heap(const vector<T> & items): m_array(new T[items.size() + 10]), currentSize(items.size()) {
		for(int i = 0; i < items.size(); i++) {
			m_array[i + 1] = items[i];
		}
	}

	// Helper functions to allow children (min/max) to manipulate Heap's private array
	template <class T, int m_size>
	T& Heap<T, m_size>::getArrayIndexValue(int index) {
		return m_array[index];
	}

	template <class T, int m_size>
	int Heap<T, m_size>::getArraySize() {
		return m_array->size();
	}

	template <class T, int m_size>
	void Heap<T, m_size>::resizeArray() {
		m_array->resize(m_array->size() * 2);
	}


#endif /* HEAP_CPP_ */

/*****************************************
** File:    MaxHeap.cpp
** Project: CSCE 221 Project 4
** Author:  Iain Anderson
** Date: 4/28/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Defines the functions laid out in its header.
**
*******************************************/

#ifndef MAX_HEAP_CPP_
#define MAX_HEAP_CPP_
#include "MaxHeap.h"


template <class T, int m_size>
MaxHeap<T, m_size>::MaxHeap(const Heap<T, m_size> &origHeap) {
    heap = new Heap<T, m_size>(origHeap);
    buildHeap();
}

template <class T, int m_size>
bool MaxHeap<T, m_size>::Contains(const T &needle) const {
    if(currentSize == 0) {
        // An empty heap contains nothing
        return false;
    }

    for (int i = 0; i < currentSize; i++) {
        if (needle == heap->getArrayIndexValue(i))
            return true;
    }
    return false;
}

template <class T, int m_size>
const T *MaxHeap<T, m_size>::Find(const T &needle) const {
    T *found;

    if (!this->Contains(needle)) {
        std::cerr << "Item not in heap." << std::endl;
        exit(1);
    }

    for (int i = 0; i < currentSize; i++) {
        if (needle == heap->getArrayIndexValue(i)) {
            found = &heap->getArrayIndexValue(i);
            break;
        }
    }
    return found;
}

template <class T, int m_size>
T& MaxHeap<T, m_size>::Remove() {
    if(currentSize == 0) {
        std::cerr << "Empty heap cannot be removed from." << std::endl;
        exit(1);
    }

    // Grab root PinHit
    T ret = T(heap->getArrayIndexValue(1).GetKey(), heap->getArrayIndexValue(1).GetValue());
 
    heap->getArrayIndexValue(1) = heap->getArrayIndexValue(currentSize--);
    PercolateDown(1);

    return ret;
}

template <class T, int m_size>
void MaxHeap<T, m_size>::Insert(T &insertable) {
    if(currentSize == heap->getArraySize() - 1)
        heap->resizeArray();

    // Calls PercolateUp to finish the job
    int index = ++currentSize;
    PercolateUp(index, insertable);
}

template <class T, int m_size>
void MaxHeap<T, m_size>::PercolateUp(int index, T& insertable) {
    for(; index >= 1 && insertable.CompareTo(heap->getArrayIndexValue(index / 2)) > 0; index /= 2)
        heap->getArrayIndexValue(index) = heap->getArrayIndexValue(index / 2); // swap from child to parent
    heap->getArrayIndexValue(index) = insertable;
}

template <class T, int m_size>
void MaxHeap<T, m_size>::PercolateDown(int index) {
    int child;
    T tmp = heap->getArrayIndexValue(index);

    for (; index * 2 <= currentSize; index = child) {
        child = index * 2;
        if (child != currentSize && heap->getArrayIndexValue(child + 1).CompareTo(heap->getArrayIndexValue(child)) > 0)
            child++; // get the right child
        if (heap->getArrayIndexValue(child).CompareTo(tmp) > 0)
            heap->getArrayIndexValue(index) = heap->getArrayIndexValue(child);
        else
            break;
    }
    heap->getArrayIndexValue(index) = tmp;
}

// A helper function for the constructor
template <class T, int m_size>
void MaxHeap<T, m_size>::buildHeap() {
    for (int i = currentSize / 2; i > 0; i--)
        PercolateDown(i);
}

#endif /* MAX_HEAP_CPP_ */

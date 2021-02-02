#include "Vect.h"
#include <cstdlib>

Vect vect_construct(unsigned int n, int val) {
    Vect v;
    v.capacity = 2 * n;
    v.size = n;
    v.arr = new int[v.capacity];
    for(unsigned int i = 0; i < n; ++i) {
        v.arr[i] = val;
    }
    return v;
}
unsigned int vect_size(const Vect &v) {
    return v.size;
}
unsigned int vect_capacity(const Vect &v) {
    return v.capacity;
}
bool vect_empty(const Vect &v) {
    if(v.size == 0)
        return true;
    else
        return false;
}
void vect_resize(Vect &v, unsigned int n) {
    v.capacity = n;
    int* temp = new int[n];
    if(v.size > n) {
        v.size = n;
    }
    for(unsigned int i = 0; i < v.size; ++i) {
        temp[i] = v.arr[i];
    }
    delete[] v.arr;
    v.arr = temp;
}
int &vect_at(Vect &v, unsigned int idx) {
    if(idx < 0 || idx > v.size-1)
        exit(1);
    return v.arr[idx];
}
void vect_erase(Vect &v, unsigned int idx) {
    if (idx < 0 || idx > v.size - 1)
        exit(1);
    int* temp = new int[v.capacity-1];
    for(unsigned int i = 0; i < v.size-1; ++i) {
        if(i < idx) {
            temp[i] = v.arr[i];
        }
        else // i >= idx
            temp[i] = v.arr[i+1];
    }
    v.size -= 1;
    delete [] v.arr;
    v.arr = temp;
}
void vect_push_back(Vect &v, int val){
    if (v.size == v.capacity)
        vect_resize(v, v.capacity * 2);
    v.arr[v.size] = val;
    v.size += 1;
}
void vect_insert(Vect &v, unsigned int idx, int val){
    if (idx < 0 || idx > v.size - 1)
        exit(1);
    if (v.size == v.capacity) {
		vect_resize(v, v.capacity * 2);
	}
	v.size += 1;
    // shifts spots from idx up one index so that val can be inserted at idx
	for (unsigned int i = v.size - 1; i > idx; --i) {
		v.arr[i] = v.arr[i - 1];
	}
	v.arr[idx] = val;
}
void vect_clear(Vect &v){
    if (v.size >= 20) {
		delete [] v.arr;
		//int* temp = new int[20];
		v.arr = new int[20];
		v.capacity = 20;
	}
	v.size = 0;
}
void vect_destruct(Vect &v){
    delete [] v.arr;
    v.arr = nullptr;
    v.size = 0;
    v.capacity = 0;
}
#include <iostream>
#include <string>
#include <cmath>
#include "StackDouble.h"

using namespace std;

// Stack::Stack() : capacity(1), length(0)

void StackDouble::push(double element) {
    if(length == capacity) {
        int newCapacity = capacity * 2;
        double *temp = new double[newCapacity];
        for (int i = 0; i < capacity; ++i)
            temp[i] = A[i];
        delete [] A;
        capacity = newCapacity;
        A = temp;
    }
    A[length] = element;
    ++length;
}

double StackDouble::pop() {
    if(length == 0) {
        return NAN;
    }
    double val = A[length - 1];
    --length;
    return val;
}

// double peek(const Stack s) {
//     return A[length - 1];
// }
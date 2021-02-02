/*
BLAS Level 1 function definitions
*/

#include "blas.h"
# include <cmath>

int amax(const double* x, const unsigned int len) {
    if (len==0) {
        return -1;
    }
    double max = fabs(x[0]);
    int maxIndex = 0;
    for(unsigned int i = 1; i < len; ++i) {
        if(fabs(x[i]) > max) {
            maxIndex = i;
        }
    }
    
    return maxIndex;
}

double asum(const double* x, const unsigned int len) {
    if(len==0) {
        return 0;
    }
    double abs_sum = 0;
    for(unsigned int i = 0; i < len; ++i) {
        abs_sum += fabs(x[i]);
    }
    
    return abs_sum;
}

void axpy(const double a, const double* x, double* y, const unsigned int len) {
    if(len != 0) {
        for(unsigned int i = 0; i < len; ++i) {
            y[i] += a * x[i];
        }
    }
}

void copy(const double* src, double* dest, const unsigned int len) {
    if(len != 0) {
        for(unsigned int i = 0; i < len; ++i) {
            dest[i] = src[i];
        }
    }
}

double dot(const double* x, const double* y, const unsigned int len) {
    if (len == 0) {
        return 0;
    }
    double sum = 0;
    for(unsigned int i = 0; i< len; ++i) {
        sum += x[i] * y[i];
    }
    return sum;
}

double norm2(const double* x, const unsigned int len) {
    if (len == 0) {
        return 0;
    }
    double sum_of_squares = 0;
    for(unsigned int i = 0; i< len; ++i) {
        sum_of_squares += pow(x[i],2);
    }
    return sqrt(sum_of_squares);
}

void scale(const double a, double* x, const unsigned int len) {
    if(len != 0) {
        for(unsigned int i = 0; i < len; ++i) {
            x[i] = a * x[i];
        }
    }
}

void swap(double* x, double* y, const unsigned int len) {
    if(len != 0) {
        for(unsigned int i = 0; i < len; ++i) {
            double temp = x[i];
            x[i] = y[i];
            y[i] = temp;
        }
    }
}
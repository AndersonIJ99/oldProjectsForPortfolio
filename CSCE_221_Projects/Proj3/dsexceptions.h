/*****************************************
** File:    dsexceptions.h
** Project: CSCE 221 Project 3
** Author:  Iain Anderson
** Date: 3/30/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Exceptions to be thrown
**
*******************************************/

#ifndef DSEXCEPTIONS_H
#define DSEXCEPTIONS_H

#include <iostream>
#include <exception>
using namespace std;

class UnderflowException : public exception {
	virtual const char* what() const throw() {
		return "UNDERFLOW EXCEPTION:";
	}
};

class ArrayIndexOutOfBounds : public exception {
	virtual const char* what() const throw() {
		return "ARRAY INDEX OUT OF BOUNDS EXCEPTION:";
	}
};

class IllegalArgumentException : public exception {
	virtual const char* what() const throw() {
		return "ILLEGAL ARGUMENT EXCEPTION:";
	}
};

class IteratorOutOfBounds : public exception {
	virtual const char* what() const throw() {
		return "ITERATOR OUT OF BOUNDS EXCEPTION:";
	}
};

class IteratorMismatchException : public exception {
	virtual const char* what() const throw() {
		return "ITERATOR MISMATCH EXCEPTION:";
	}
};

class IteratorUninitializedException : public exception
{
	virtual const char *what() const throw()
	{
		return "ITERATOR UNINITIALIZED EXCEPTION:";
	}
};

#endif
/*****************************************
** File:    Exceptions.h
** Project: CSCE 221 Project 2
** Author:  Iain Anderson
** Date: 2/23/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Exceptions to be thrown
**
*******************************************/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <exception>
using namespace std;

class ItemNotFound : public exception {
	virtual const char* what() const throw() {
		return "ITEM NOT FOUND EXCEPTION:";
	}
};

class CommandNotFound : public exception {
	virtual const char* what() const throw() {
		return "INVALID COMMAND EXCEPTION:";
	}
};

class InvalidArgumentException : public exception {
	virtual const char* what() const throw() {
		return "INVALID ARGUMENT EXCEPTION:";
	}
};

class NodeNotFoundException : public exception {
	virtual const char* what() const throw() {
		return "NODE NOT FOUND EXCEPTION:";
	}
};

class NULLArgumentException : public exception {
	virtual const char* what() const throw() {
		return "NULL ARGUMENT EXCEPTION:";
	}
};

#endif
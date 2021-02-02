/*****************************************
** File:    Exceptions.h
** Project: CSCE 221 Project 2
** Author:  Iain Anderson
** Date: 2/23/2020   
** Section: 518
** E-mail: iain@tamu.edu 
**
**  Exceptions will be handled through here.
**
*******************************************/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <exception>
#include <string>
using namespace std;

class Exceptions
{
	public:
		Exceptions(string message) : m_message(message) {}
		string GetMessage() { return m_message; }
	private:
		string m_message;
};

#endif
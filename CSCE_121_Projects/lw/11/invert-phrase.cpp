#include <iostream>
#include <string>
#include "Stack.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
    Stack<string> s;

    // YOUR CODE GOES HERE
	// cout << "Starting" << endl;
	string input;
	//cin >> input;
	getline(cin, input);
	
	string subStr = input;
	
	while (subStr.find_first_of(" ") != -1)
	{
		int indxOfSpace = subStr.find_first_of(" ");
		string temp = subStr.substr(0, indxOfSpace);
		subStr = subStr.substr(indxOfSpace + 1);
		
		//cout << "In while loop:" << temp << endl;
		s.push(temp);

		
	}
	
	int str_length = subStr.length();
	
	subStr = subStr.substr(0, str_length - 1);
	//cout << subStr << endl;
	s.push(subStr);
	
	while(!s.isEmpty()){
		cout << s.pop() << " ";
	}
	
	cout << endl;
    
    return 0;
}

#ifndef STACK_H
#define STACK_H


class StackDouble {
public:
	// Stack();
	void push(double);
	double pop();
	// double peek(const Stack);

private:
	int capacity = 1;
	int length = 0;
	double* A = new double[1];
};


#endif
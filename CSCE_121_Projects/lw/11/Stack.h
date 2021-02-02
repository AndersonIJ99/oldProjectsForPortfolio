#ifndef STACK_H
#define STACK_H

template <class T>
class Stack
{
public:
	// Stack();
	~Stack() {
		delete[] A;
	}
	void push(T element) {
		if (length == capacity)
		{
			int newCapacity = capacity * 2;
			T *temp = new T[newCapacity];
			for (int i = 0; i < capacity; ++i)
				temp[i] = A[i];
			delete[] A;
			capacity = newCapacity;
			A = temp;
		}
		A[length] = element;
		++length;
	}
	T pop() {
		if (length == 0)
		{
			exit(0);
		}
		T val = A[length - 1];
		--length;
		return val;
	}
	bool isEmpty() {
		if (length == 0)
			return true;
		else
			return false;
	}
	// double peek(const Stack);

private:
	int capacity = 1;
	int length = 0;
	T *A = new T[1];
};

#endif
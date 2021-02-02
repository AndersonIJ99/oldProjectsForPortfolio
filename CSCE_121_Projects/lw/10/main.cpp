#include <iostream>
#include "Vect.h"

using namespace std;

int main()
{
	Vect v1 = vect_construct(10,2);
	cout << vect_size(v1);
	return 0;
}
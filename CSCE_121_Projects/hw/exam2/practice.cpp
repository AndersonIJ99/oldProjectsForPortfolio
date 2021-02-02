#include <iostream>
using namespace std;

// int increaseArray(int *&array, int size, int value)
// {
//     int newSize = size + 5;
//     int* newArray = new int[newSize];
//     if(size != 0) {
//         for (int i = 0; i < size; ++i)
//         {
//             newArray[i] = array[i];
//         }
//     }
//     for (int j = size; j < newSize; ++j)
//     {
//         newArray[j] = value;
//     }
//     delete [] array;
//     size = newSize;
//     array = newArray;
//     //newArray = nullptr;
//     return size;
// }

// void printArray(int* array, int size)
// {
//     if (size == 0 || array == nullptr)
//     {
//         cout << "Empty List" << endl;
//     }
//     else
//     {
//         for (int i = 0; i < size; ++i)
//         {
//             cout << array[i] << " ";
//         }
//         cout << endl;
//     }
// }

// int main()
// { // test program
//     int* testArray = nullptr;
//     int testSize = 0;
//     int testValue = 12;
//     testSize = increaseArray(testArray, testSize, testValue);
//     printArray(testArray, testSize);
// }

// int main() {
//     int no_rows = 2;
//     int no_cols = 4;
//     int arr[no_rows][no_cols];
//     int i = 0, j = 0;
//     // while(i < no_rows) {
//     //     arr[i][j] = i * no_cols + j;
//     //     j += 1;
//     //     if(j == no_cols) {
//     //         i += 1;
//     //     }
//     //     if(j % no_cols == 0)
//     //         j = 0;
//     // }
//     for(int r = 0; r < no_rows; ++r) {
//         for(int c = 0; c < no_cols; ++c) {
//             arr[r][c] = r * no_cols + c;
//         }
//     }

//     for(int r = 0; r < no_rows; ++r) {
//         for(int c = 0; c < no_cols; ++c) {
//             cout << arr[r][c] << " ";
//         }
//         cout << endl;
//     }
// }

// int f1(int, int&);
// int f2(int&);

// int main() {
//     int a = 2;
//     int b = 9;
//     int c = f1(a,b);
//     cout << "a = " << a << endl;
//     cout << "b = " << b << endl;
//     cout << "c = " << c;
// }

// int f1(int r, int& s) {
//     r *= 3;
//     s /= 4;
//     int temp = r + f2(s);
//     return temp;
// }

// int f2(int& p) {
//     p = 12;
//     return p;
// }

// int countEqual(int* numbers, int n, int x) {
//     if(n==-1)
//         return 0;
//     if(numbers[n] == x)
//         return 1 + countEqual(numbers, n - 1, x);
//     else
//         return 0 + countEqual(numbers, n - 1, x);
// }

// double dist(double* u, double *v, int n) {
//     if(n==-1)
//         return 0;
//     double d = pow(u[n] - v[n],2) + dist(u, v, n - 1);
//     return sqrt(d);
// }

// double continousFraction(double* x, int n) {
//     if((1+x[0]) == 0)
//         return -1;
//     if(n -2 == 0)
//         return 1.0 + x[1]/(1.0 + x[0]);
//     if(n - 2 > 0)
//         return 1.0 + x[n-1]/continousFraction(x, n-2);
// }

// int main() {
//     double* u = new double[2];
//     // double *v = new double[2];
//     int n = 2;
//     u[0] = 10.0;
//     u[1] = 3.0;
//     u[2] = 2.0;
//     // v[0] = 1;
//     // v[1] = 2;
//     // v[2] = 3;
//     cout << continousFraction(u,n);
// }

main() {
    cout << rand() / RAND_MAX;
}
#include <iostream>
#include "functions.h"
using namespace std;

int main()
{
    int a = 0, b = 0, count = 0, numRepeat = 0;

    cout << "Enter numbers a <= b: ";
    cin >> a >> b;

    if ((a < 1) || (b < 1) || (a > b) || (b > 10000)) {
        cout << "Invalid input" << endl;
        return 0;
    }
    
    for (int i = a; i <= b; ++i) { 
        // covers the range of numbers between a and b, inclusive
        for (int j = 0; j <= 9; ++j) { 
            // Checks if 0-9 is repeated in any of the numbers in the range
            if (countDigitOccurrences(i, j) > 1) { 
                // totals up duplicate numbers
                numRepeat += 1;
                break;
            }
        }
        count += 1;
    }
    
    cout << "There are " << count - numRepeat << " valid numbers between " << a << " and " << b << endl;

    return 0;
}
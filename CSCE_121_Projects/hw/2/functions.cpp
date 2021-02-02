# include <iostream>
# include "functions.h"
using namespace std;

int countDigitOccurrences(int n, int digit) { 
    // return number of occurences of digit in n
    int count = 0;
    // This is an edge case solution for when the user enters zero for a
    if(n == 0 && digit == 0)
        return 1;
    // This is an edge case where the number being checked is less than 10 
    // and doesn't need multiple numerical places checked
    if(n < 10 && n == digit)
        return 1;
    
    // checks n's numerical places checking for occurrences of digit in n
    while(n > 0) {
        int possibleDigit = n % 10;
        if(possibleDigit == digit) {
            count += 1;
        }
        n /= 10;
    }
    
    return count;
}
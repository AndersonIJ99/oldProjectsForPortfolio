# include <iostream>
using namespace std;

int main() {
    int numDollars, numCents, numQuarters, numDimes, numNickels, numPennies = 0;
    // Gathers dollars and cents as integers to avoid floating point math errors
    cout << "Enter dollars: ";
    cin >> numDollars;
    cout << "Enter cents: ";
    cin >> numCents;
    // Calculates the amount of each coin type and reduces the next amount to calculate appropriately
    numQuarters = numDollars * 4 + numCents / 25;
    numCents %= 25;
    numDimes = numCents / 10;
    numCents %= 10;
    numNickels = numCents / 5;
    numCents %= 5;
    numPennies = numCents;
    int totalCoins = numQuarters + numDimes + numNickels + numPennies; // Totals the coins counted

    cout << "Pennies: " << numPennies << endl;
    cout << "Nickels: " << numNickels << endl;
    cout << "Dimes: " << numDimes << endl;
    cout << "Quarters: " << numQuarters << endl;
    cout << endl << "Total coins used: " << totalCoins << endl;

    return 0;
}
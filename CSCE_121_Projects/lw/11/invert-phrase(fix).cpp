#include <iostream>
#include <string>
#include "Stack.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
    Stack<string> s;

    string order;
    getline(cin, order);

    int i = 0;
    int start = 0;
    while(order[i] != '.') {
        s.push(order.substr(start,order.find(' ', start)));
        start = order.find(' ', start);
        ++i;
    }
    // s.push(order.substr(start + 1, order.find('.') - 1));
    cout << s.pop() << s.pop();

    return 0;
}

#include <iostream>
#include "Node.h"
#include "DoublyLinkedList.h"

using namespace std;

int main()
{
    DoublyLinkedList<int> dl = DoublyLinkedList<int>();
    dl.push_back(5);
    dl.push_back(3);
    dl.insert(4,1);
    dl.erase(2);
    DoublyLinkedList<int> dl2 = dl;
    // dl.pop_back();
    cout << dl << endl;
    if(dl==dl2)
        cout << dl2 << endl;
    // cout << dl2 << endl;
    return 0;
}
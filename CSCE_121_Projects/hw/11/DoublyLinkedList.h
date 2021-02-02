#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H
#include "Node.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

//////////////////////////////////////////////////////////////////////////////
//                         class template definition                        //
//////////////////////////////////////////////////////////////////////////////

template<typename T>
class DoublyLinkedList {
public:
    /* uncomment the declarations as you implement them */
    DoublyLinkedList();
    DoublyLinkedList(T);
    DoublyLinkedList(const DoublyLinkedList<T>&);
    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>&);
    ~DoublyLinkedList();
    unsigned int size() const { return sz; }
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void push_back(T);
    void push_front(T);
    void pop_back();
    void pop_front();
    void erase(unsigned int);
    void insert(T data, unsigned int idx);
    void clear();
    std::string to_str() const;
    // breaks encapsulation... gives operator== access to private data... not great approach, but makes the homework easier for you since we haven't talked about iterators :).
    template<typename U> friend bool operator==(DoublyLinkedList<U> const&, DoublyLinkedList<U> const&);
private:
    // do not change ordering.
    Node<T> *head;
    Node<T> *tail;
    unsigned int sz;
};

//////////////////////////////////////////////////////////////////////////////
//                       helper function declarations                       //
//////////////////////////////////////////////////////////////////////////////
template<typename T> std::ostream& operator<<(std::ostream &, DoublyLinkedList<T> const&);

template<typename T> bool operator==(DoublyLinkedList<T> const&, DoublyLinkedList<T> const&);

template<typename T> bool operator!=(DoublyLinkedList<T> const&, DoublyLinkedList<T> const&);

//////////////////////////////////////////////////////////////////////////////
//                     member template function definitions                 //
//////////////////////////////////////////////////////////////////////////////
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
    this->head = nullptr;
    this->tail = nullptr;
    this->sz = 0;
}
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(T data) : head(nullptr), tail(nullptr), sz(0)
{
    head = new Node<T>(data);
    tail = head;
    sz += 1;
}
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& rhs) : head(nullptr), tail(nullptr), sz(0) {
    Node<T>* curr = rhs.head;
    while (curr) {
        this->push_back(curr->data);
        curr = curr->next;
    }
}
template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList<T>& rhs) {
    if (this != &rhs) {
        clear();
        Node<T>* curr = rhs.head;
        while (curr) {
            this->push_back(curr->data);
            curr = curr->next;
        }
    }
    return *this;
}
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}
template <typename T>
T& DoublyLinkedList<T>::front() {
    return head->data;
}
template <typename T>
const T& DoublyLinkedList<T>::front() const{
    return head->data;
}
template <typename T>
T& DoublyLinkedList<T>::back() {
    return tail->data;
}
template <typename T>
const T& DoublyLinkedList<T>::back() const{
    return tail->data;
}
template <typename T>
void DoublyLinkedList<T>::push_back(T data) {
    if(head == nullptr) {
        head = new Node<T>(data);
        tail = head;
        sz += 1;
        return;
    }
    Node<T>* n = new Node<T>(data);
    tail->next = n;
    n->prev = tail;
    tail = n;
    sz += 1;
}
template <typename T>
void DoublyLinkedList<T>::push_front(T data) {
    if(head == nullptr) {
        head = new Node<T>(data);
        tail = head;
        sz += 1;
        return;
    }
    Node<T>* n = new Node<T>(data);
    head->prev = n;
    n->next = head;
    head = n;
    sz += 1;
}
template <typename T>
void DoublyLinkedList<T>::pop_back() {
    if(head == tail) {
        //when there is one node in the list
        delete head;
        head = nullptr;
        tail = nullptr;
    }
    else {
        Node<T> *temp = tail->prev;
        delete tail;
        tail = temp;
        temp->next = nullptr;
    }
    sz -= 1;
}
template <typename T>
void DoublyLinkedList<T>::pop_front() {
    if(head == tail) {
        //when there is one node in the list
        delete head;
        head = nullptr;
        tail = nullptr;
    }
    else {
        Node<T> *temp = head->next;
        delete head;
        head = temp;
        temp->prev = nullptr;
    }
    sz -= 1;
}
template <typename T>
void DoublyLinkedList<T>::erase(unsigned int idx) {
    if(idx > sz)
        exit(0);
    if(idx == 0) {
        pop_front();
        return;
    }
    if(idx == sz - 1) {
        pop_back();
        return;
    }
    Node<T>* curr = head;
    for(unsigned int i = 0; i < idx; ++i)
        curr = curr->next;
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    delete curr;
    sz -= 1;
}
template <typename T>
void DoublyLinkedList<T>::insert(T data, unsigned int idx) {
    if(idx >= sz)
        exit(0);
    if(idx == 0) {
        push_front(data);
        return;
    }
    Node<T>* curr = head;
    for(unsigned int i = 0; i < idx - 1; ++i)
        curr = curr->next;
    Node<T>* n = new Node<T>(data);
    n->next = curr->next;
    n->prev = curr;
    curr->next->prev = n;
    curr->next = n;
    sz += 1;
}
template <typename T>
void DoublyLinkedList<T>::clear() {
    if(head == tail) {
        // only 1 element in the list
        delete head;
    }
    else {
        Node<T>* curr = head;
        while(curr != nullptr) {
            Node<T>* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
    }
    head = nullptr;
    tail = nullptr;
    sz = 0;
}
template <typename T>
std::string DoublyLinkedList<T>::to_str() const
{
    std::stringstream os;
    const Node<T> *curr = head;
    os << std::endl << std::setfill('-') << std::setw(80) << '-' << std::setfill(' ') << std::endl;
    os << "head: " << head << std::endl;
    os << "tail: " << tail << std::endl;
    os << "  sz: " << sz << std::endl;
    os << std::setw(16) << "node" << std::setw(16) << "node.prev" << std::setw(16) << "node.data" <<  std::setw(16) << "node.next" << std::endl;
    while (curr) {
        os << std::setw(16) << curr;
        os << std::setw(16) << curr->prev;
        os << std::setw(16) << curr->data;
        os << std::setw(16) << curr->next;
        os << std::endl;
        curr = curr->next;
    }
    os << std::setfill('-') << std::setw(80) << '-' << std::setfill(' ') << std::endl;
    return os.str();
}

//////////////////////////////////////////////////////////////////////////////
//                     helper template function definitions                 //
//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator==(DoublyLinkedList<T> const& lhs, DoublyLinkedList<T> const& rhs)
{
    if(lhs.size() != rhs.size())
        return false;
    // element wise comparison of lhs and rhs lists
    Node<T>* iter_rhs = rhs.head;
    Node<T>* iter_lhs = lhs.head;
    while (iter_rhs) {
        if(iter_lhs->data != iter_rhs->data)
            return false;
        iter_lhs = iter_lhs->next;
        iter_rhs = iter_rhs->next;
    }
    return true;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, DoublyLinkedList<T> const& rhs)
{
    os << rhs.to_str();
    return os;
}

#endif

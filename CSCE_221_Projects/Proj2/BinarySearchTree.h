#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_

#include <iostream>       // For NULL
#include <queue>
// #include "Exceptions.h"
#include "AugmentedBinarySearchTree.h"
using namespace std;

// Binary node and forward declaration because g++ does
// not understand nested classes.
template <class Comparable>
class BinarySearchTree;

#ifndef BINARY_NODE
#define BINARY_NODE
template <class Comparable>
class BinaryNode
{
	Comparable element;
	BinaryNode *left;
	BinaryNode *right;
	int m_size;

	BinaryNode(const Comparable & theElement = -1, BinaryNode *lt = NULL, BinaryNode *rt = NULL, int size = -1)
		: element(theElement), left(lt), right(rt), m_size(size)  { }
	friend class BinarySearchTree<Comparable>;
	friend class AugmentedBinarySearchTree<Comparable>;

};
#endif // BINARY_NODE


// BinarySearchTree class
//
// CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// Comparable find( x )   --> Return item that matches x
// Comparable findMin( )  --> Return smallest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order

template <class Comparable>
class BinarySearchTree
{
public:
	explicit BinarySearchTree();
	BinarySearchTree(const BinarySearchTree<Comparable> & rhs);
	~BinarySearchTree();
	const Comparable & findMin() const;
	const Comparable & find(const Comparable & x) const;
	bool isEmpty() const;
	void printTree() const;
	void makeEmpty();
	void insert(const Comparable & x);
	void remove(const Comparable & x);
	const BinarySearchTree & operator=(const BinarySearchTree & rhs);
private:
	BinaryNode<Comparable> *root;
	const Comparable & elementAt(BinaryNode<Comparable> *t) const;
	void insert(const Comparable & x, BinaryNode<Comparable> * & t) const; 
	void remove(const Comparable & x, BinaryNode<Comparable> * & t) const;
	BinaryNode<Comparable> * findMin(BinaryNode<Comparable> *t) const;
	BinaryNode<Comparable> * find(const Comparable & x, BinaryNode<Comparable> *t) const;
	void makeEmpty(BinaryNode<Comparable> * & t) const;
	void printTree(BinaryNode<Comparable> *t) const;
	BinaryNode<Comparable> * clone(BinaryNode<Comparable> *t) const;
};

#include "BinarySearchTree.cpp"
#endif


#ifndef BinarySearchTree_C
#define BinarySearchTree_C

#include "BinarySearchTree.h"
#include <iostream>
using namespace std;

//const int NUM_NODES_PER_LINE = 6; //For formatting

/**
 * Implements an unbalanced binary search tree.
 * Note that all "matching" is based on the < method.
 */

/**
 * Construct the tree.
 */
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree() :
root(NULL)
{
	//no code
}


/**
 * Copy constructor.
 */
template <class Comparable>
BinarySearchTree<Comparable>::
BinarySearchTree(const BinarySearchTree<Comparable> & rhs) :
root(NULL)
{
	*this = rhs;
}

/**
 * Destructor for the tree.
 */
template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree()
{
	makeEmpty();
}

/**
 * Insert x into the tree; duplicates are ignored.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x)
{
	insert(x, root);
}

/**
* Remove x from the tree. return 1 on success o.w. return 0
*/
template <class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x)
{
	remove(x, root);
}
/**
 * Find the smallest item in the tree.
 * Return smallest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin() const
{
	return elementAt(findMin(root));
}

/**
 * Find item x in the tree.
 * Return the matching item or throw an exception if not found
 */
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::
find(const Comparable & x) const
{
	return elementAt(find(x, root));
}

/**
 * Make the tree logically empty.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty()
{
	makeEmpty(root);
}

/**
 * Test if the tree is logically empty.
 * Return true if empty, false otherwise.
 */
template <class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const
{
	return root == NULL;
}

/**
 * Print the tree contents in sorted order.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::printTree() const
{
	if (isEmpty())
		cout << "Empty tree" << endl;
	else
		printTree(root);
}

/**
 * Deep copy.
 */
template <class Comparable>
const BinarySearchTree<Comparable> &
BinarySearchTree<Comparable>::
operator=(const BinarySearchTree<Comparable> & rhs)
{
	if (this != &rhs)
	{
		makeEmpty();
		root = clone(rhs.root);
	}
	return *this;
}


/**
 * Internal method to get element field in node t.
 * Return the element field or throw exception if t is NULL.
 */
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::
elementAt(BinaryNode<Comparable> *t) const
{
	if (t == NULL)
		throw ItemNotFound();
	else
		return t->element;

}

/**
* Internal method to insert into a subtree.
* x is the item to insert.
* t is the node that roots the subtree.
* Set the new root of the subtree.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::
insert(const Comparable & x, BinaryNode<Comparable> * & t) const
{
	if (t == NULL)
		t = new BinaryNode<Comparable>(x, NULL, NULL);
	else if (x < t->element)
		insert(x, t->left);
	else if (t->element < x)
		insert(x, t->right);
	else 
		; // Duplicates
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the tree.
 * Set the new root.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::
remove(const Comparable & x, BinaryNode < Comparable > * & t) const
{

	if (t == NULL)
		return;   // Item not found; do nothing
	if (x < t->element)
		remove(x, t->left);
	else if (t->element < x)
		remove(x, t->right);
	else if (t->left != NULL && t->right != NULL) // Two children
	{
		t->element = findMin(t->right)->element;
		remove(t->element, t->right);
	}
	else
	{
		BinaryNode <Comparable> *oldNode = t;
		t = (t->left != NULL) ? t->left : t->right;
		delete oldNode;
	}

}


/**
 * Internal method to find the smallest item in a subtree t.
 * Return node containing the smallest item.
 */
template <class Comparable>
BinaryNode<Comparable> *
BinarySearchTree<Comparable>::findMin(BinaryNode<Comparable> *t) const
{
	if (t == NULL)
		return NULL;
	if (t->left == NULL)
		return t;
	return findMin(t->left);
}


/**
 * Internal method to find an item in a subtree.
 * x is item to search for.
 * t is the node that roots the tree.
 * Return node containing the matched item.
 */
template <class Comparable>
BinaryNode<Comparable> *
BinarySearchTree<Comparable>::
find(const Comparable & x, BinaryNode<Comparable> *t) const
{
	if (t == NULL)
		return NULL;
	else if (x < t->element)
		return find(x, t->left);
	else if (t->element < x)
		return find(x, t->right);
	else
		return t;    // Match
}
/****** NONRECURSIVE VERSION*************************
		template <class Comparable>
		BinaryNode<Comparable> *
		BinarySearchTree<Comparable>::
		find( const Comparable & x, BinaryNode<Comparable> *t ) const
		{
		while( t != NULL )
		if( x < t->element )
		t = t->left;
		else if( t->element < x )
		t = t->right;
		else
		return t;    // Match

		return NULL;   // No match
		}
		*****************************************************/

/**
 * Internal method to make subtree empty.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::
makeEmpty(BinaryNode<Comparable> * & t) const
{
	if (t != NULL)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t = NULL;
}

/**
 * Internal method to print a subtree rooted at t in sorted order.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::printTree(BinaryNode<Comparable> *t) const
{
	if (t != NULL)
	{
		printTree(t->left);
		cout << t->element << endl;
		printTree(t->right);
	}
}

/**
 * Internal method to clone subtree.
 */
template <class Comparable>
BinaryNode<Comparable> *
BinarySearchTree<Comparable>::clone(BinaryNode<Comparable> * t) const
{
	if (t == NULL)
		return NULL;
	else
		return new BinaryNode<Comparable>(t->element, clone(t->left), clone(
		t->right));
}




#endif

/*****************************************
** File:    AugmentedBinarySearchTree.h
** Project: CSCE 221 Project 2
** Author:  Iain Anderson
** Date:    2/23/2020
** Section: 518
** E-mail:  iain@tamu.edu
**
**  C++ file:
**	This file defines the functions to be used by  
**  an augmented BST.
**	
**
*******************************************/

using namespace std;

#ifndef AUGMENTED_BINARY_SEARCH_TREE_C
#define AUGMENTED_BINARY_SEARCH_TREE_C

/*---------------Imported Libraries---------------*/
#include <fstream>
#include <sstream>
#include <iostream> // For NULL
#include <cmath> // for ceil
#include <queue>

/*----------------Includes---------------------*/

#include "AugmentedBinarySearchTree.h"

/*------------Constructors/Destructors-------------*/

/**
 * Construct the tree.
 */
template <class Comparable>
AugmentedBinarySearchTree<Comparable>::AugmentedBinarySearchTree() : root(NULL) {
    //no code
}

/**
 * Copy constructor.
 */
template <class Comparable>
AugmentedBinarySearchTree<Comparable>::AugmentedBinarySearchTree(const AugmentedBinarySearchTree<Comparable> &rhs) : root(NULL) {
    *this = rhs;
}

/**
 * Destructor for the tree.
 */
template <class Comparable>
AugmentedBinarySearchTree<Comparable>::~AugmentedBinarySearchTree() {
    makeEmpty();
}

/*--------------Facilitators------------------------*/

/**
* Remove x from the tree.
*/
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::remove(const Comparable &x) {
	return remove(x, root);
}

/**
* Returns if the BST is perfect or not.
*/
template <class Comparable>
bool AugmentedBinarySearchTree<Comparable>::IsPerfect() {
    // Instantiation of a queue to be passed to the worker function
    queue<BinaryNode<Comparable>*> q;
    
    // If the tree is not empty push the root onto the queue
    if (root != NULL) {
        q.push(root);
    }
    
    // Get the depth of the tree with the height function
    int height = getHeight(root);
    
    // Pass the queue and height to the worker to get results
    return IsPerfect(q, height);
}

// template <class Comparable>
// bool AugmentedBinarySearchTree<Comparable>::IsComplete(); -> extra credit

/**
* Prints the levels of the BST as specified by the call.
*/
template <class Comparable>
void AugmentedBinarySearchTree<Comparable>::PrintLevels(int numlevels) {
    // Print nothing if there is no tree
    if (root == NULL)
        return;
    // Instantiate a queue to pass to the worker function
    queue<BinaryNode<Comparable> *> q;
    // Push the root onto the queue
    q.push(root);
    
    return PrintLevels(q, numlevels);
}

/**
 * Make the tree logically empty.
 */
template <class Comparable>
void AugmentedBinarySearchTree<Comparable>::makeEmpty() {
    // An empty tree doesn't need to made empty
    if(root == NULL)
        return;
	makeEmpty(root);
}

/*---------------Getters---------------------------*/

/**
* Remove residual nodes created during PrintLevels().
*/
/* Assume RemoveResidue will always be called after Print */
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::RemoveResidue() {
    // Keep track of the number of deletions
    int deletions = 0;
    RemoveResidue(root, &deletions);
    return deletions;
}

/**
 * Calls internal method to find the Nth node.
 * Then returns the Nth node's element.
 */
template <class Comparable>
const Comparable& AugmentedBinarySearchTree<Comparable>::NthElement(int n) {
    const Comparable& ret = -1;
    // No tree
    if(root == NULL) {
        cerr << "Error: Empty tree" <<  endl;
        return ret;
    }

    // Out of bounds error
    if(n < 0 || n > root->m_size) {
        cerr << "Error: Out of bounds Nth Element." << endl;
        return ret;
    }

    int nodesVisited = 1;
    return NthElement(root, &nodesVisited, n)->element;
}

/**
 * Calls internal method to find the rank of x.
 * Then returns its rank if x is in the BST.
 */
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::Rank(const Comparable &x) {
    // nodesVisited keeps track of the rank of the node we are looking for in the tree
    int nodesVisited = 1;
    Rank(x, root, &nodesVisited);
    return nodesVisited;
}

/**
 * Returns the median element of a BST.
 */
template <class Comparable>
const Comparable& AugmentedBinarySearchTree<Comparable>::Median() {
    const Comparable& ret = -1;
    if (root == NULL) {
        cerr << "An empty tree does not have a median." << endl;
        return ret;
    }
    // Get the middle location
    // Use ceiling to index properly
    int size = root->m_size;
    int medianLoc = ceil((double) size / 2);
    return NthElement(medianLoc);
}

/**
 * Internal method to find the smallest item in a subtree t.
 * Return node containing the smallest item.
 */
template <class Comparable>
BinaryNode<Comparable> *AugmentedBinarySearchTree<Comparable>::findMin(BinaryNode<Comparable> *t) const {
    if(t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

/*---------------Setters---------------------------*/

/**
 * Insert x into the tree; duplicates are ignored.
 */
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::insert(const Comparable &x) {
	return insert(x, root);
}

/**
* Internal method to insert into a subtree.
* x is the item to insert.
* t is the node that roots the subtree.
* Set the new root of the subtree.
*/
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::insert(const Comparable &x, BinaryNode<Comparable> *&t) const {
	if (t == NULL) {
		t = new BinaryNode<Comparable>(x, NULL, NULL, 1);
        return 1; // 1 for successful insertion
    }
	else if (x < t->element) {
		t->m_size += 1;
		return insert(x, t->left);
	}
	else if (t->element < x) {
		t->m_size += 1;
		return insert(x, t->right);
	}
	else {
        refactor(root);
        return 0; // 0 for duplicate insertion
    }
        
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the tree.
 * Set the new root.
 */
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::remove(const Comparable &x, BinaryNode<Comparable> *&t) const
{
    if (t == NULL) {
        refactor(root);
        return 0; // x not found or no tree so nothing changed
    }

    // Decrement m_size as we traverse
    t->m_size -= 1;

    if (x < t->element) {
        return remove(x, t->left);
    }
    else if (t->element < x) {
        return remove(x, t->right);
    }
    else if (t->left != NULL && t->right != NULL) // Two children
    {
        t->element = findMin(t->right)->element;
        return remove(t->element, t->right);
    }
    else
    {
        BinaryNode<Comparable> *oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
        return 1; // 1 for successful deletion
    }
}

/**
* Internal method for printing the levels of the tree as specified
* by its call.
*/
template <class Comparable>
void AugmentedBinarySearchTree<Comparable>::PrintLevels(queue<BinaryNode<Comparable> *> q, int levels) {
    // Pointer instantiation to access node data for printing
    BinaryNode<Comparable>* nodeVar = NULL;
    
    // Useful variable initialization
    int i = 0;
    int j = 0;
    int nodesVisited = 0;
    int depth = 0;
    int size = pow(2, levels) - 1;
    // Contains the parents in the tree for access later
    int parents[size];
    
    // Continue while there are still children/nodes to print
    while (!q.empty()) {
        // Get the front node
        nodeVar = q.front();
        // Dequeue it (pop off the queue)
        q.pop();

        // Visited a node so increment
        nodesVisited += 1;
        if (nodesVisited == 1) {
            cout << "Level " << depth << ": " << endl;
        }

        // If we are at the front of the queue
        if (depth == 0) {
            // Print the node's data
            cout << "(" << nodeVar->element << ", " << nodeVar->m_size << ", NULL) ";
        }
        else if (nodeVar->element < 0) {
            // Get the value of the parents of the current node if they are any
            bool hasParentItems = parents[i] > 0;
            if (!hasParentItems) {
                // No parents after check so print null
                cout << "(NULL, NULL, NULL)";
            }
            else {
                // Child and leaf nodes
                cout << "(NULL, NULL, "<< parents[i] << ")";
            }
            cout << ' ';
        }
        else {
            cout << "(" << nodeVar->element << ", ";
            cout << nodeVar->m_size << ", " << parents[i] << ")";
        }

        // Add new parents to the parents array
        parents[j++] = nodeVar->element;
        
        // If we aren't at the depth
        if(levels - 1 != depth) {
            // If no left child create a placeholder
            if (nodeVar->left == NULL) {
                BinaryNode<Comparable>* temp = new BinaryNode<Comparable>();
                nodeVar->left = temp;
                q.push(temp);
            }
            // Otherwise enqueue the child
            else {
                q.push(nodeVar->left);
            }
            
            // Enqueue right child
            if (nodeVar->right != NULL) {
                q.push(nodeVar->right);
            }
            // Otherwise create a placeholder
            else
            {
                BinaryNode<Comparable>* temp = new BinaryNode<Comparable>();
                nodeVar->right = temp;
                q.push(temp);
            }
        }

        // Check for six nodes per line or even
        bool divBySix = nodesVisited % 6 == 0;
        bool isEven = nodesVisited % 2 == 0;
        
        // If we are at six then go to next line to print more nodes
        if (divBySix)
            cout << endl;
        if (isEven) 
            i += 1;

        // Check is we are at the end of the current tree level
        if (pow(2, depth) == nodesVisited) {
            depth += 1;
            nodesVisited = 0;
            cout << "\n" << endl;
        }

        // Add a space between node data
        cout << ' ';
    }
}

/**
* Internal method that deletes residual nodes created during
* the PrintLevels operation.
*/
template <class Comparable>
void AugmentedBinarySearchTree<Comparable>::RemoveResidue(BinaryNode<Comparable> *&t, int *deletions) const {
    try {
        // If the tree is empty throw an exception
        if (t == NULL) {
            NULLArgumentException e;
            throw e;
        }

        // check if left & right are valid
        if (t->left != NULL)
            RemoveResidue(t->left, deletions);
        if (t->right != NULL)
            RemoveResidue(t->right, deletions);

        // If the node is a residual then remove it and increment the number of deletions
        // Residuals have a value of -1
        if (t->element == -1) {
            *deletions -= -1;
            delete t;
            t = NULL;
        }
    } catch (NULLArgumentException) {
        // catch error
        cerr << "Invalid/NULL Argument in RemoveResidue" << endl;
        return;
    }
}

/**
 * Internal method that finds the Nth node for the public method.
 */
template <class Comparable>
BinaryNode<Comparable>* AugmentedBinarySearchTree<Comparable>::NthElement(BinaryNode<Comparable> *t, int *nodesVisited, int n) const {
    // A temporary node will allow us to traverse the nodes until we have reached the Nth node at which we will return
    // Check for left child (left != NULL)
    if (t->left) {
        // Check to the left for the Nth node
        BinaryNode<Comparable>* tempNode = NthElement(t->left, nodesVisited, n);
        if (tempNode != NULL) 
            return tempNode;
    }
    
    // If we are at the Nth node return it
    if (*nodesVisited == n)
        return t;

    // decrement by 1	
    *nodesVisited -= -1;
    
    // Go right and check
    if (t->right) {
        // Check to the right for the Nth node
        BinaryNode<Comparable>* tempNode = NthElement(t->right, nodesVisited, n);
        if (tempNode != NULL) 
            return tempNode;
    }
    
    return NULL;
}

/**
 * Internal method that finds x and its rank.
 */
template <class Comparable>
void AugmentedBinarySearchTree<Comparable>::Rank(const Comparable &x, BinaryNode<Comparable> *t, int *nodesVisited) const {
    // Check for NULL
    if (t == NULL) {
        cerr << "ERROR: Node is NULL" << endl;
        return;
    }
    
    // Determine the size of the left subtree for traversal purposes
    int leftSize = (t->left == NULL) ? 0 : t->left->m_size;

    // Holds nodes traversed based on where we start in the tree
    int mySize = leftSize + 1;

    // If we need to go right
    if (x > t->element) {
        // Go right
        *nodesVisited += mySize;
        Rank(x, t->right, nodesVisited);
    }
    // If we need to go left
    else if (x < t->element) {
        // Go left
        Rank(x, t->left, nodesVisited);
    }
    // x == t->element
    else {
        if (t->left == NULL) {
            return;
        } else {
            // Increase "rank" holding variable by number of nodes traversed
            *nodesVisited += leftSize;
        }
    }
    
    return;
}

/**
* Internal method that figures out if the BST is perfect.
*/
template <class Comparable>
bool AugmentedBinarySearchTree<Comparable>::IsPerfect(queue<BinaryNode<Comparable> *> q, int height) {
    // Create a pointer to handle iteration
    BinaryNode<Comparable>* nodeVar = NULL;
    
    // Useful variable initialization
    int i = 0;
    int nodesVisited = 0;
    int depth = 0;
    
    // Continue while there are still children/nodes to check
    while (!q.empty()) {
        // Get the front node
        nodeVar = q.front();
        // Dequeue it (pop off the queue)
        q.pop();

        // Visited a node so increment
        nodesVisited += 1;

        if(height - 1 != depth) {
            // If no left child create a placeholder
            if (nodeVar->left == NULL) {
                BinaryNode<Comparable>* temp = new BinaryNode<Comparable>();
                nodeVar->left = temp;
                q.push(temp);
            }
            // Otherwise enqueue the child
            else {
                q.push(nodeVar->left);
            }
            
            // Enqueue right child
            if (nodeVar->right != NULL) {
                q.push(nodeVar->right);
            }
            // Otherwise create a placeholder
            else
            {
                BinaryNode<Comparable>* temp = new BinaryNode<Comparable>();
                nodeVar->right = temp;
                q.push(temp);
            }
        }
        // Check for even
        bool isEven = nodesVisited % 2 == 0;
        if (isEven) 
            i += 1;

        // Check to make sure we are perfect so far
        if (pow(2, depth) == nodesVisited) {
            depth += 1;
            nodesVisited = 0;
        }
    }

    // Determine the difference in the node count between a tree
    // with residue possibly added and a perfect tree  
    int difference = RemoveResidue();

    // return if the difference is 0
    // if so the tree must be perfect with no space for residual nodes
    // otherswise the tree is not perfect
    return difference == 0;
}

/**
 * Internal method to make subtree empty.
 */
template <class Comparable>
void AugmentedBinarySearchTree<Comparable>::makeEmpty(BinaryNode<Comparable> *&t) const
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
 * Helper function that determines the height of the tree.
 */
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::getHeight(BinaryNode<Comparable>* node) {
    // No height for NULL
    if (node == NULL)
        return 0;
    // Determine the left sub-tree's size
    int leftHeight = getHeight(node->left);
    // Determine the right sub-tree's size
    int rightHeight = getHeight(node->right);

    // Return the larger of the two + 1
    int largest = max(leftHeight, rightHeight);
    return largest + 1;
}

/**
 * Helper function that fixes m_size for nodes
 * affected by the failed insertion of a duplicate.
 */
template <class Comparable>
void AugmentedBinarySearchTree<Comparable>::refactor(BinaryNode<Comparable> *t) const {
    if (t != NULL)
    {
        refactor(t->left);
        t->m_size = size(t);
        refactor(t->right);
    }
}

/**
 * Helper function that calculates the size of
 * subtrees plus the current node for refactor method.
 */
template <class Comparable>
int AugmentedBinarySearchTree<Comparable>::size(BinaryNode<Comparable> *t) const {
    if(t == NULL)
        return 0;
    else
       return(size(t->left) + 1 + size(t->right));
}

// template <class Comparable>
//IsComplete  <-- Extra Credit!


#endif // AUGMENTED_BINARY_SEARCH_TREE_C
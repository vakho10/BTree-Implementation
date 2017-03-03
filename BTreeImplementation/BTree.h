#ifndef BTREE_H
#define BTREE_H

#pragma once

#include "BTreeNode.h"

template<class T>
class BTree
{
	BTreeNode<T> *root; // Pointer to root node
	int t;  // Minimum degree
public:
	// Constructor (Initializes tree as empty)
	BTree(int _t)
	{
		root = NULL;  t = _t;
	}

	// function to traverse the tree
	void traverse()
	{
		if (root != NULL) root->traverse();
	}

	// function to search a key in this tree
	BTreeNode<T>* search(T k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	// The main function that inserts a new key in this B-Tree
	void insert(T k);
};

template<class T>
void BTree<T>::insert(T k)
{
	// If tree is empty
	if (root == NULL)
	{
		// Allocate memory for root
		root = new BTreeNode<T>(t, true);
		root->keys[0] = k;  // Insert key
		root->n = 1;  // Update number of keys in root
	}
	else // If tree is not empty
	{
		// If root is full, then tree grows in height
		if (root->n == 2 * t - 1)
		{
			// Allocate memory for new root
			BTreeNode<T> *s = new BTreeNode<T>(t, false);

			// Make old root as child of new root
			s->C[0] = root;

			// Split the old root and move 1 key to the new root
			s->splitChild(0, root);

			// New root has two children now.  Decide which of the
			// two children is going to have new key
			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			// Change root
			root = s;
		}
		else  // If root is not full, call insertNonFull for root
			root->insertNonFull(k);
	}
}

#endif

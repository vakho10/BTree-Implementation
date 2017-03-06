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

	// The main function that removes a new key in thie B-Tree
	void remove(T k);
};

#endif
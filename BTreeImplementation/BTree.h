#ifndef BTREE_H
#define BTREE_H

#pragma once

#include <set>
#include <string>

#include "BTreeNode.h"

template<typename T, typename Compare = std::less<T>>
class BTree
{
	BTreeNode<T, Compare> *root; // Pointer to root node
	int t;  // Minimum degree
	Compare cmp; // Custom comparator
public:
	// Constructor (Initializes tree as empty)
	BTree(int _t, Compare& _cmp = Compare())
	{
		root = NULL;  
		t = _t;
		cmp = _cmp;
	}

	// function to traverse the tree
	void traverse(std::string(*printableValue)(T*))
	{
		if (root != NULL) root->traverse(printableValue);
	}

	// function to search a key in this tree
	BTreeNode<T, Compare>* search(T* k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	// The main function that inserts a new key in this B-Tree
	void insert(T* k);

	// The main function that removes a new key in thie B-Tree
	void remove(T* k);
};

#endif
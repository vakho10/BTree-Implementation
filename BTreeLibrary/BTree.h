#pragma once

#include <string>

#include "BTreeNode.h"
#include "Functions.h" // Contains comparators and extractors

template<typename T, typename Extract = PlaintKeyExtractor<T>, typename Compare = Less<T, Extract> >
class BTree
{
	BTreeNode<T, Extract, Compare> *root; // Pointer to root node
	int t;  // Minimum degree
	Extract ext; // Custom key extractor
	Compare cmp; // Custom comparator
public:
	// Constructor (Initializes tree as empty)
	BTree(int _t, Extract& _ext = Extract(), Compare& _cmp = Compare())
	{
		root = NULL;  
		t = _t;
		ext = _ext;
		cmp = _cmp;
	}

	// function to traverse the tree
	void traverse()
	{
		if (root != NULL) root->traverse();
	}

	// function to search a key in this tree
	BTreeNode<T, Extract, Compare>* search(T& k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	// The main function that inserts a new key in this B-Tree
	void insert(T& k);

	// The main function that removes a new key in thie B-Tree
	void remove(T& k);
};
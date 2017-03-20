#pragma once

#include <string>

template<typename T, typename Extract, typename Compare>
class BTreeNode
{
	T **keys;  // An array of keys (objects)
	int t;      // Minimum degree (defines the range for number of keys)
	BTreeNode<T, Extract, Compare> **C; // An array of child pointers
	int n;     // Current number of keys
	bool leaf; // Is true when node is leaf. Otherwise false
	Extract ext;
	Compare cmp;
public:
	BTreeNode(int _t, bool _leaf, Extract& _ext = Extract(), Compare& _cmp = Compare());   // Constructor

	// A function to traverse all nodes in a subtree rooted with this node
	void traverse();

	// A function to search a key in subtree rooted with this node.
	BTreeNode<T, Extract, Compare> *search(T& k);   // returns NULL if k is not present.

	// A function that returns the index of the first key that is greater
	// or equal to k
	int findKey(T& k);

	// A utility function to insert a new key in the subtree rooted with
	// this node. The assumption is, the node must be non-full when this
	// function is called
	void insertNonFull(T& k);

	// A utility function to split the child y of this node. i is index
	// of y in child array C[].  The Child y must be full when this
	// function is called
	void splitChild(int i, BTreeNode<T, Extract, Compare> *y);

	// A wrapper function to remove the key k in subtree rooted with
	// this node.
	void remove(T& k);

	// A function to remove the key present in idx-th position in
	// this node which is a leaf
	void removeFromLeaf(int idx);

	// A function to remove the key present in idx-th position in
	// this node which is a non-leaf node
	void removeFromNonLeaf(int idx);

	// A function to get the predecessor of the key- where the key
	// is present in the idx-th position in the node
	T* getPred(int idx);

	// A function to get the successor of the key- where the key
	// is present in the idx-th position in the node
	T* getSucc(int idx);

	// A function to fill up the child node present in the idx-th
	// position in the C[] array if that child has less than t-1 keys
	void fill(int idx);

	// A function to borrow a key from the C[idx-1]-th node and place
	// it in C[idx]th node
	void borrowFromPrev(int idx);

	// A function to borrow a key from the C[idx+1]-th node and place it
	// in C[idx]th node
	void borrowFromNext(int idx);

	// A function to merge idx-th child of the node with (idx+1)th child of
	// the node
	void merge(int idx);

	// Make BTree friend of this so that we can access private members of this
	// class in BTree functions
	template<typename T, typename Extract, typename Compare> friend class BTree;
};
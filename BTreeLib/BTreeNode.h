#pragma once

#include <string>
#include <functional>

namespace BTreeLib
{
	// Should be visible to NodePair!
	template<typename T, typename Compare>
	class BTreeNode;

	template<typename T, typename Compare = std::less<T>>
	class NodePair
	{
	private:
		T key;
		BTreeNode<T, Compare> *child;
		Compare cmp;
	public:
		NodePair(T _key, Compare& _cmp = Compare())
		{
			key = _key;
			cmp = _cmp;
		}

		NodePair(T _key, BTreeNode<T, Compare>* _child, Compare& _cmp = Compare()) : NodePair(_key, _cmp) 
		{
			child = _child;
		}

		~NodePair() 
		{
			delete key; key = NULL;
			delete child; child = NULL;
		}

		T getKey() { return key; }
		BTreeNode<T, Compare>* getChild() { return child; }

		void setKey(T _key) { key = _key; }
		void setChild(BTreeNode<T, Compare>* _child) { child = _child; }
	};

	template<typename T, typename Compare = std::less<T>>
	class BTreeNode
	{
		Compare cmp;

		bool leaf;					// Is true when node is leaf. Otherwise false
		int t;						// Minimum degree (defines the range for number of keys)

		NodePair<T, Compare>** pairs; // Array of pairs consisting of keys and children
		BTreeNode<T, Compare>* c_last = NULL; // ბოლოს კვანძის მარჯვენა შვილი (მაშინ ვიყენებთ როდესაც კვანძი სავსეა)

		int positionOfFirstKey;		// კვანძში პირველი გასაღების ინდექსი
		int n;						// Current number of keys
		int ndCapacity;				// კვანძში გასაღებების შესაძლო რაოდენობის მაქსიმუმი ლუწი უნდა იყოს და 2-ის ხარისხიც

	public:
		BTreeNode(int _t, bool _leaf, Compare& _cmp = Compare());   // Constructor
		~BTreeNode();

		T* getKeys() { return this->keys; }
		int getCapacity() { return this->ndCapacity; }
		int getPositionOfFirstKey() { return positionOfFirstKey; }

		int find_ind_inNode(BTreeNode<T, Compare>* x, T k, int st, int fin)
		{
			if (st >= fin) return st;
			int middle = (st + fin) / 2;
			if (cmp(x->pairs[middle % x->ndCapacity]->getKey(), k))  return find_ind_inNode(x, k, middle + 1, fin);
			return find_ind_inNode(x, k, st, middle);
		}

		// A function to traverse all nodes in a subtree rooted with this node
		std::string traverse();

		// A function to search a key in subtree rooted with this node.
		BTreeNode<T, Compare> *search(T k);   // returns NULL if k is not present.

		// A function that returns the index of the first key that is greater
		// or equal to k
		int findKey(T k);

		// A utility function to insert a new key in the subtree rooted with
		// this node. The assumption is, the node must be non-full when this
		// function is called
		void insertNonFull(T k);

		// A utility function to split the child y of this node. i is index
		// of y in child array C[].  The Child y must be full when this
		// function is called
		int splitChild(int i, BTreeNode<T, Compare> *y);

		// A wrapper function to remove the key k in subtree rooted with
		// this node.
		void remove(T k);

		// A function to remove the key present in idx-th position in
		// this node which is a leaf
		void removeFromLeaf(int idx);

		// A function to remove the key present in idx-th position in
		// this node which is a non-leaf node
		void removeFromNonLeaf(int idx);

		// A function to get the predecessor of the key- where the key
		// is present in the idx-th position in the node
		T getPred(int idx);

		// A function to get the successor of the key- where the key
		// is present in the idx-th position in the node
		T getSucc(int idx);

		// A function to fill up the child node present in the idx-th
		// position in the C[] array if that child has less than t-1 keys
		int fill(int idx);

		// A function to borrow a key from the C[idx-1]-th node and place
		// it in C[idx]th node
		void borrowFromPrev(int idx);

		// A function to borrow a key from the C[idx+1]-th node and place it
		// in C[idx]th node
		void borrowFromNext(int idx);

		// A function to merge idx-th child of the node with (idx+1)th child of
		// the node. returns true if merge happend in last two children!
		int merge(int idx);

		// Make BTree friend of this so that we can access private members of this
		// class in BTree functions
		template<typename T, typename Compare> friend class BTree;
	};
}
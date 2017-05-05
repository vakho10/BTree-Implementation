#pragma once

#include <string>
#include <functional>

namespace BTreeLib
{
	template<typename T, typename Compare = std::less<T>>
	class BTreeNode
	{
		Compare cmp;

		bool leaf;					// Is true when node is leaf. Otherwise false
		int t;						// Minimum degree (defines the range for number of keys)

		T *keys;					// An array of keys (objects)
		BTreeNode<T, Compare> **C;	// An array of child pointers

		int positionOfFirstKey;		// კვანძში პირველი გასაღების ინდექსი 
		int n;						// Current number of keys
		int ndCapacity;				// კვანძში გასაღებების შესაძლო რაოდენობის მაქსიმუმი ლუწი უნდა იყოს და 2-ის ხარისხიც

	public:
		BTreeNode(int _t, bool _leaf, Compare& _cmp = Compare());   // Constructor
		~BTreeNode();	

		int getPositionOfFirstKey() {
			return positionOfFirstKey;
		}

		// დამხმარე ფუნქცია რომელიც ეძებს ჩასასმელი რიცხვის ადგილს
		int find_ind_inNode(BTreeNode<T, Compare> *x, T k, int st, int fin, int ndCapacity)
		{
			int md, i = st;
			while (st <= fin)
			{
				md = (st + fin) / 2;
				if (k == keys[md])
				{
					i = md;
					fin = md - 1; // Goto left side and find another match
				}
				else
					if (cmp(k, keys[md])) // k < keys[middle]
					{
						fin = md - 1;
						i = fin;
					}
					else {
						st = md + 1;
						i = st;
					}
			}
			return i;
			//// თუ ერთელემენტეიანიას შემთხვევა
			//if (x->n == 1) return x->keys[x->positionOfFirstKey] < k ? x->positionOfFirstKey + 1 : x->positionOfFirstKey;
			//
			//if (st == fin) return st;
			//int middle = (st + fin) / 2;
			//if (k > x->keys[middle % ndCapacity])  return find_ind_inNode(x, k, middle + 1, fin, ndCapacity);
		}

		// A function to traverse all nodes in a subtree rooted with this node
		void traverse();

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
		void splitChild(int i, BTreeNode<T, Compare> *y);

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
		template<typename T, typename Compare> friend class BTree;
	};
}
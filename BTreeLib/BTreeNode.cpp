#include "BTreeNode.h"

namespace BTreeLib
{
	template<typename T, typename Compare>
	BTreeNode<T, Compare>::BTreeNode(int _t, bool _leaf, Compare& _cmp = Compare())
	{
		// Copy the given minimum degree and leaf property
		t = _t;
		leaf = _leaf;

		// Set ndCapacity according to t parameter
		ndCapacity = 2 * _t - 1; // FIXME should be odd and power of 2?!
		positionOfFirstKey = 0;
		keysNumber = 0;

		// Allocate memory for maximum number of possible keys
		// and child pointers
		keys = (T*)malloc(sizeof(T)*(ndCapacity)); // new T[2 * t - 1];
		C = new BTreeNode<T, Compare> *[2 * t];

		// Initialize the number of keys as 0
		n = 0;

		cmp = _cmp;
	}

	template<typename T, typename Compare>
	BTreeNode<T, Compare>::~BTreeNode()
	{
		free(keys);
		keys = NULL;
	}

	// A utility function that returns the index of the first key that is
	// greater than or equal to k
	template<typename T, typename Compare>
	int BTreeNode<T, Compare>::findKey(T k)
	{
		// Find the first key greater than or equal to k
		int left = positionOfFirstKey, right = positionOfFirstKey + n - 1, middle, i = positionOfFirstKey;
		while (left <= right)
		{
			middle = (left + right) / 2;
			if (k == keys[middle % ndCapacity])
			{
				i = middle;
				right = middle - 1; // Goto left side and find another match
			}
			else
				if (cmp(k, keys[middle % ndCapacity])) // k < keys[middle]
					right = middle - 1;
				else {
					left = middle + 1;
					i = left;
				}
		}
		return i;
	}

	// A function to remove the key k from the sub-tree rooted with this node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::remove(T k)
	{
		int idx = findKey(k); // index that may be beyond array size!
		int absIdx = idx % ndCapacity; // Absolute (real) index

		// The key to be removed is present in this node
		if (idx < n + positionOfFirstKey && keys[absIdx] == k) // idx < n && keys[idx] == k!
		{
			// If the node is a leaf node - removeFromLeaf is called
			// Otherwise, removeFromNonLeaf function is called
			if (leaf)
				removeFromLeaf(idx);
			else
				removeFromNonLeaf(idx);
		}
		else
		{
			// If this node is a leaf node, then the key is not present in tree
			if (leaf)
			{
				cout << "The key " << k << " does not exist in the tree\n";
				return;
			}

			// The key to be removed is present in the sub-tree rooted with this node
			// The flag indicates whether the key is present in the sub-tree rooted
			// with the last child of this node
			bool flag = ((idx == n + positionOfFirstKey) ? true : false);

			// If the child where the key is supposed to exist has less that t keys,
			// we fill that child
			if (C[absIdx]->n < t)
				fill(absIdx);

			// If the last child has been merged, it must have merged with the previous
			// child and so we recurse on the (idx-1)th child. Else, we recurse on the
			// (idx)th child which now has atleast t keys
			if (flag && idx > n + positionOfFirstKey)
				C[absIdx - 1]->remove(k);
			else
				C[absIdx]->remove(k);
		}
		return;
	}

	// A function to remove the idx-th key from this node - which is a leaf node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::removeFromLeaf(int idx)
	{
		int st = positionOfFirstKey;
		int fin = st + n - 1;

		if (-1 == idx) // Index can't be less than 0
			return;

		int ind = idx % ndCapacity;

		if (ind - st > fin - ind)
		{
			while (fin != ind)
			{
				keys[ind % ndCapacity] = keys[(ind + 1) % ndCapacity];
				++ind;
			}
		}
		else
		{
			while (st != ind)
			{
				keys[ind % ndCapacity] = keys[(ind - 1) % ndCapacity];
				--ind;
			}
			positionOfFirstKey = (st + 1) % ndCapacity;
		}

		// Reduce the count of keys
		--n;
		
		// Move all the keys after the idx-th pos one place backward (legacy code)
		/*for (int i = idx + 1; i < n; ++i)
			keys[i - 1] = keys[i];*/
		return;
	}

	// A function to remove the idx-th key from this node - which is a non-leaf node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::removeFromNonLeaf(int idx)
	{
		int absIdx = idx % ndCapacity; // Change index to absolute index
		int childArrIdx = idx % (ndCapacity + 1);

		T k = keys[absIdx];

		// If the child that precedes k (C[idx]) has atleast t keys,
		// find the predecessor 'pred' of k in the subtree rooted at
		// C[idx]. Replace k by pred. Recursively delete pred
		// in C[idx]
		if (C[childArrIdx]->n >= t)
		{
			T pred = getPred(idx);
			keys[absIdx] = pred;
			C[childArrIdx]->remove(pred);
		}

		// If the child C[idx] has less that t keys, examine C[idx+1].
		// If C[idx+1] has atleast t keys, find the successor 'succ' of k in
		// the subtree rooted at C[idx+1]
		// Replace k by succ
		// Recursively delete succ in C[idx+1]
		else if (C[(idx + 1) % (ndCapacity + 1)]->n >= t) // FIXME slow slow slow!!!!!
		{
			T succ = getSucc(idx);
			keys[absIdx] = succ;
			C[(idx + 1) % (ndCapacity + 1)]->remove(succ);
		}

		// If both C[idx] and C[idx+1] has less that t keys, merge k and all of C[idx+1]
		// into C[idx]
		// Now C[idx] contains 2t-1 keys
		// Free C[idx+1] and recursively delete k from C[idx]
		else
		{
			merge(idx);
			C[childArrIdx]->remove(k);
		}
		return;
	}

	// A function to get predecessor of keys[idx]
	template<typename T, typename Compare>
	T BTreeNode<T, Compare>::getPred(int idx)
	{
		// Keep moving to the right most node until we reach a leaf
		BTreeNode<T, Compare> *cur = C[idx];
		while (!cur->leaf)
			cur = cur->C[cur->n];

		// Return the last key of the leaf
		return cur->keys[cur->n - 1];
	}

	template<typename T, typename Compare>
	T BTreeNode<T, Compare>::getSucc(int idx)
	{

		// Keep moving the left most node starting from C[idx+1] until we reach a leaf
		BTreeNode<T, Compare> *cur = C[idx + 1];
		while (!cur->leaf)
			cur = cur->C[0];

		// Return the first key of the leaf
		return cur->keys[0];
	}

	// A function to fill child C[idx] which has less than t-1 keys
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::fill(int idx)
	{

		// If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
		// from that child
		if (idx != 0 && C[idx - 1]->n >= t)
			borrowFromPrev(idx);

		// If the next child(C[idx+1]) has more than t-1 keys, borrow a key
		// from that child
		else if (idx != n && C[idx + 1]->n >= t)
			borrowFromNext(idx);

		// Merge C[idx] with its sibling
		// If C[idx] is the last child, merge it with with its previous sibling
		// Otherwise merge it with its next sibling
		else
		{
			if (idx != n)
				merge(idx);
			else
				merge(idx - 1);
		}
		return;
	}

	// A function to borrow a key from C[idx-1] and insert it
	// into C[idx]
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::borrowFromPrev(int idx)
	{
		BTreeNode<T, Compare> *child = C[idx];
		BTreeNode<T, Compare> *sibling = C[idx - 1];

		// The last key from C[idx-1] goes up to the parent and key[idx-1]
		// from parent is inserted as the first key in C[idx]. Thus, the  loses
		// sibling one key and child gains one key

		// Moving all key in C[idx] one step ahead
		for (int i = child->n - 1; i >= 0; --i)
			child->keys[i + 1] = child->keys[i];

		// If C[idx] is not a leaf, move all its child pointers one step ahead
		if (!child->leaf)
		{
			for (int i = child->n; i >= 0; --i)
				child->C[i + 1] = child->C[i];
		}

		// Setting child's first key equal to keys[idx-1] from the current node
		child->keys[0] = keys[idx - 1];

		// Moving sibling's last child as C[idx]'s first child
		if (!leaf)
			child->C[0] = sibling->C[sibling->n];

		// Moving the key from the sibling to the parent
		// This reduces the number of keys in the sibling
		keys[idx - 1] = sibling->keys[sibling->n - 1];

		child->n += 1;
		sibling->n -= 1;

		return;
	}

	// A function to borrow a key from the C[idx+1] and place
	// it in C[idx]
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::borrowFromNext(int idx)
	{
		BTreeNode<T, Compare> *child = C[idx];
		BTreeNode<T, Compare> *sibling = C[idx + 1];

		// keys[idx] is inserted as the last key in C[idx]
		child->keys[(child->n)] = keys[idx];

		// Sibling's first child is inserted as the last child
		// into C[idx]
		if (!(child->leaf))
			child->C[(child->n) + 1] = sibling->C[0];

		//The first key from sibling is inserted into keys[idx]
		keys[idx] = sibling->keys[0];

		// Moving all keys in sibling one step behind
		for (int i = 1; i < sibling->n; ++i)
			sibling->keys[i - 1] = sibling->keys[i];

		// Moving the child pointers one step behind
		if (!sibling->leaf)
		{
			for (int i = 1; i <= sibling->n; ++i)
				sibling->C[i - 1] = sibling->C[i];
		}

		// Increasing and decreasing the key count of C[idx] and C[idx+1]
		// respectively
		child->n += 1;
		sibling->n -= 1;

		return;
	}

	// A function to merge C[idx] with C[idx+1]
	// C[idx+1] is freed after merging
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::merge(int idx)
	{
		BTreeNode<T, Compare> *child = C[idx % (ndCapacity + 1)]; // C[idx]
		BTreeNode<T, Compare> *sibling = C[(idx + 1) % (ndCapacity + 1)]; // C[idx + 1]

		// Pulling a key from the current node and inserting it into (t-1)th
		// position of C[idx]
		child->keys[(positionOfFirstKey + (t - 1)) % ndCapacity] = keys[idx % ndCapacity];

		// Copying the keys from C[idx+1] to C[idx] at the end
		for (int i = positionOfFirstKey; i < sibling->n + positionOfFirstKey; ++i)
			child->keys[(i + t) % ndCapacity] = sibling->keys[i % ndCapacity]; // FIXME slowest operation!

		// Copying the child pointers from C[idx+1] to C[idx]
		if (!child->leaf)
		{
			for (int i = positionOfFirstKey; i <= sibling->n + positionOfFirstKey; ++i)
				child->C[(i + t) % (ndCapacity + 1)] = sibling->C[i % (ndCapacity + 1)]; // FIXME slowest operation!
		}

		// Moving all keys after idx in the current node one step before -
		// to fill the gap created by moving keys[idx] to C[idx]
		for (int i = idx + 1; i < n + positionOfFirstKey; ++i)
			keys[(i - 1) % ndCapacity] = keys[i % ndCapacity];

		// Moving the child pointers after (idx+1) in the current node one
		// step before
		for (int i = idx + 2; i <= n + positionOfFirstKey; ++i)
			C[(i - 1) % (ndCapacity + 1)] = C[i % (ndCapacity + 1)];

		// Updating the key count of child and the current node
		child->n += sibling->n + 1;
		n--;

		// Freeing the memory occupied by sibling
		delete(sibling);
		return;
	}

	// A utility function to insert a new key in this node
	// The assumption is, the node must be non-full when this
	// function is called
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::insertNonFull(T k)
	{
		// Initialize index as index of rightmost element
		int i = positionOfFirstKey + n - 1;

		// If this is a leaf node
		if (leaf == true)
		{
			// The following loop does two things
			// a) Finds the location of new key to be inserted
			// b) Moves all greater keys to one place ahead
			while (i >= positionOfFirstKey && cmp(k, keys[i % ndCapacity])) // keys[i] > k
			{
				keys[(i + 1) % ndCapacity] = keys[i % ndCapacity];
				i--;
			}

			// Insert the new key at found location
			keys[(i + 1) % ndCapacity] = k;
			n = n + 1;
		}
		else // If this node is not leaf
		{
			// Find the child which is going to have the new key
			while (i >= positionOfFirstKey && cmp(k, keys[i % ncCapacity])) // keys[i] > k
				i--;

			// See if the found child is full
			int index = (i + 1) % ndCapacity
			int indexForChildArray = (i + 1) % (ndCapacity + 1);
			if (C[indexForChildArray]->n == 2 * t - 1)
			{
				// If the child is full, then split it
				splitChild(index, C[indexForChildArray]); // 1) Position of the new key (in existing node); 2) Child which has to be splitted

				// After split, the middle key of C[i] goes up and
				// C[i] is splitted into two.  See which of the two
				// is going to have the new key
				if (cmp(keys[index], k)) { // keys[i + 1] < k
					i++;
					indexForChildArray = i % (ndCapacity + 1);
				}
			}
			C[indexForChildArray]->insertNonFull(k);
		}
	}

	// A utility function to split the child y of this node
	// Note that y must be full when this function is called
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::splitChild(int i, BTreeNode<T, Compare> *y)
	{
		// Create a new node which is going to store (t-1) keys of y
		BTreeNode<T, Compare> *z = new BTreeNode<T, Compare>(y->t, y->leaf);
		z->n = t - 1;

		// FIXME speed decrease because of many % calculations?!

		// Copy the last (t-1) keys of y to z
		for (int j = 0; j < t - 1; j++)
			z->keys[j] = y->keys[(y->positionOfFirstKey + j + t) % ndCapacity]; // z has it as it is: indexed normally, from zero to end

		// Copy the last t children of y to z
		if (y->leaf == false)
		{
			for (int j = 0; j < t; j++)
				z->C[j] = y->C[(y->positionOfFirstKey + j + t) % (ndCapacity + 1)]; // Normal indexing for Z's children
		}

		// Reduce the number of keys in y
		y->n = t - 1;

		// Since this node is going to have a new child,
		// create space of new child
		for (int j = n + positionOfFirstKey; j >= i + 1; j--)
			C[(j + 1) % (ndCapacity + 1)] = C[j % (ndCapacity + 1)];

		// Link the new child to this node
		C[(i + 1) % (ndCapacity + 1)] = z;

		// A key of y will move to this node. Find location of
		// new key and move all greater keys one space ahead
		for (int j = n + positionOfFirstKey - 1; j >= i; j--)
			keys[(j + 1) % ndCapacity] = keys[j % ndCapacity]; // TODO check if this slow operation might be changed to faster?!

		// Copy the middle key of y to this node
		keys[i] = y->keys[(t + positionOfFirstKey - 1) % ndCapacity];

		// Increment count of keys in this node
		n = n + 1;
	}

	// Function to traverse all nodes in a subtree rooted with this node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::traverse()
	{
		// There are n keys and n+1 children, travers through n keys
		// and first n children
		int indexOfChildArray;
		for (int i = positionOfFirstKey; i < n + positionOfFirstKey; i++)
		{
			int index = i % ndCapacity;
			indexOfChildArray = i % (ndCapacity + 1);

			// If this is not leaf, then before printing key[i],
			// traverse the subtree rooted with child C[i].
			if (leaf == false)
				C[indexOfChildArray]->traverse();
			cout << " " << keys[index];
		}

		// Print the subtree rooted with last child
		if (leaf == false)
			C[indexOfChildArray]->traverse();
	}

	// Function to search key k in subtree rooted with this node
	template<typename T, typename Compare>
	BTreeNode<T, Compare> *BTreeNode<T, Compare>::search(T k)
	{
		// Find the first key greater than or equal to k
		int left = positionOfFirstKey, right = positionOfFirstKey + n - 1, middle, i = positionOfFirstKey;
		while (left <= right)
		{
			middle = (left + right) / 2;
			if (k == keys[middle % ndCapacity])
			{
				i = middle;
				right = middle - 1; // Goto left side and find another match
			}
			else
				if (cmp(k, keys[middle % ndCapacity])) // k < keys[middle]
					right = middle - 1;
				else {
					left = middle + 1;
					i = left;
				}
		}

		// If the found key is equal to k, return this node
		if (keys[i % ndCapacity] == k) // keys[i] == k!
			return this;

		// If key is not found here and this is a leaf node
		if (leaf == true)
			return NULL;

		// Go to the appropriate child
		return C[i % ndCapacity]->search(k);
	}
}
#include "BTreeNode.h"

#include <string>
#include <sstream>

using namespace std;

namespace BTreeLib
{
	template<typename T, typename Compare>
	BTreeNode<T, Compare>::BTreeNode(int _t, bool _leaf, Compare& _cmp = Compare())
	{
		// Copy the given minimum degree and leaf property
		t = _t;
		leaf = _leaf;

		// Set ndCapacity according to t parameter
		ndCapacity = 2 * t - 1; // FIXME should be odd and power of 2?!
		positionOfFirstKey = 0;

		// Allocate memory for maximum number of possible keys and child pointers
		keys = (T*)malloc(sizeof(T)*(ndCapacity)); // new T[2 * t - 1];
		C = new BTreeNode<T, Compare>*[2 * t - 1]; // same size as keys

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
		//int idx = positionOfFirstKey;
		//while (idx < n + positionOfFirstKey && cmp(keys[idx % ndCapacity], k)) // keys[idx] < k
		//	++idx;

		// Find the first key greater than or equal to k
		/*int left = positionOfFirstKey, right = positionOfFirstKey + n - 1, middle, i = positionOfFirstKey;
		while (left <= right)
		{
			middle = (left + right) / 2;
			if (k == keys[middle % ndCapacity])
			{
				i = middle;
				right = middle - 1;
				break;
			}
			else
				if (k < keys[middle % ndCapacity]) {
					i = middle;
					right = middle - 1;
				}
				else {
					left = middle + 1;
					i = left;
				}
		}*/
		return find_ind_inNode(this, k, positionOfFirstKey, positionOfFirstKey + n - 1);
	}

	// A function to remove the key k from the sub-tree rooted with this node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::remove(T k)
	{
		int idx = findKey(k);

		if (keys[idx % ndCapacity] < k) idx++; // ინდექსის გასწორება!
					
		// The key to be removed is present in this node
		if (idx < positionOfFirstKey + n && keys[(idx + ndCapacity) % ndCapacity] == k) // keys[idx] == k!
		{
			// If the node is a leaf node - removeFromLeaf is called
			// Otherwise, removeFromNonLeaf function is called
			if (leaf)
				removeFromLeaf(idx); // TODO done. needs speedup!
			else
				removeFromNonLeaf(idx);
		}
		else
		{
			// If this node is a leaf node, then the key is not present in tree
			if (leaf)
			{
				cout << "The key " << k << " is does not exist in the tree\n";
				return;
			}

			// The key to be removed is present in the sub-tree rooted with this node
			// The flag indicates whether the key is present in the sub-tree rooted
			// with the last child of this node
			bool flag = ((idx == n + positionOfFirstKey) ? true : false);

			BTreeNode<T, Compare>* idxChild = flag ? c_last : C[idx % ndCapacity];

			// If the child where the key is supposed to exist has less that t keys,
			// we fill that child
			if (idxChild->n < t)
				fill(idx);

			// If the last child has been merged, it must have merged with the previous
			// child and so we recurse on the (idx-1)th child. Else, we recurse on the
			// (idx)th child which now has atleast t keys
			if (flag && idx > n + positionOfFirstKey)
				C[(idx - 1) % ndCapacity]->remove(k);
			else
				idxChild->remove(k);
		}
		return;
	}

	// A function to remove the idx-th key from this node - which is a leaf node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::removeFromLeaf(int idx)
	{
		// Move all the keys after the idx-th pos one place backward
		for (int i = idx + 1; i < n + positionOfFirstKey; ++i)
			keys[(i - 1) % ndCapacity] = keys[i % ndCapacity];

		// Reduce the count of keys
		n--;
		return; 
	}

	// A function to remove the idx-th key from this node - which is a non-leaf node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::removeFromNonLeaf(int idx)
	{
		T k = keys[idx % ndCapacity];

		// წინასწარ განსაზღვრე მარჯვენა შვილი არის თუ არა ბოლო
		BTreeNode<T, Compare>* rightChild = (idx + 1 != positionOfFirstKey + n) ? C[(idx + 1) % ndCapacity] : c_last;
		
		// If the child that precedes k (C[idx]) has atleast t keys,
		// find the predecessor 'pred' of k in the subtree rooted at
		// C[idx]. Replace k by pred. Recursively delete pred
		// in C[idx]
		if (C[idx % ndCapacity]->n >= t)
		{
			T pred = getPred(idx);
			keys[idx % ndCapacity] = pred;
			C[idx % ndCapacity]->remove(pred);
		}

		// If the child C[idx] has less that t keys, examine C[idx+1].
		// If C[idx+1] has atleast t keys, find the successor 'succ' of k in
		// the subtree rooted at C[idx+1]
		// Replace k by succ
		// Recursively delete succ in C[idx+1]
		else if (rightChild->n >= t)
		{
			T succ = getSucc(idx);
			keys[idx % ndCapacity] = succ;
			rightChild->remove(succ);
		}

		// If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
		// into C[idx]
		// Now C[idx] contains 2t-1 keys
		// Free C[idx+1] and recursively delete k from C[idx]
		else
		{		
			bool wasLast = merge(idx);
			
			// თუ ბოლოები შეერთდა
			if (wasLast) {
				c_last = C[(positionOfFirstKey + n) % ndCapacity]; 
				c_last->remove(k);
			} else
				C[idx % ndCapacity]->remove(k);
		}
		return;
	}

	// A function to get predecessor of keys[idx]
	template<typename T, typename Compare>
	T BTreeNode<T, Compare>::getPred(int idx)
	{
		// Keep moving to the right most node until we reach a leaf
		BTreeNode<T, Compare> *cur = C[idx % ndCapacity];
		while (!cur->leaf)
			cur = cur->c_last;

		// Return the last key of the leaf
		return cur->keys[(cur->positionOfFirstKey + cur->n - 1) % ndCapacity];
	}

	template<typename T, typename Compare>
	T BTreeNode<T, Compare>::getSucc(int idx)
	{
		// Keep moving the left most node starting from C[idx+1] until we reach a leaf
		BTreeNode<T, Compare> *cur = (idx + 1 != positionOfFirstKey + n) ? C[(idx + 1) % ndCapacity] : c_last;
		while (!cur->leaf)
			cur = cur->C[cur->positionOfFirstKey];

		// Return the first key of the leaf
		return cur->keys[cur->positionOfFirstKey];
	}

	// A function to fill child C[idx] which has less than t-1 keys
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::fill(int idx)
	{
		bool flag = ((idx == n + positionOfFirstKey) ? true : false);
		// TODO speed up!
		BTreeNode<T, Compare>* rightChild = flag ? c_last : C[(idx + 1) % ndCapacity];

		// If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
		// from that child
		if (idx != positionOfFirstKey && C[(idx - 1) % ndCapacity]->n >= t)
			borrowFromPrev(idx);

		// If the next child(C[idx+1]) has more than t-1 keys, borrow a key
		// from that child
		else if (idx != n + positionOfFirstKey && rightChild->n >= t)
			borrowFromNext(idx);

		// Merge C[idx] with its sibling
		// If C[idx] is the last child, merge it with with its previous sibling
		// Otherwise merge it with its next sibling
		else
		{
			bool mergeHappenedInLastTwo = false;

			if (idx != positionOfFirstKey + n)
				mergeHappenedInLastTwo = merge(idx);
			else
				mergeHappenedInLastTwo = merge(idx - 1);
			
			if (mergeHappenedInLastTwo) c_last = C[(positionOfFirstKey + n) % ndCapacity]; // ბოლო შვილის გასწორება
			
		}
		return;
	}

	// A function to borrow a key from C[idx-1] and insert it
	// into C[idx]
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::borrowFromPrev(int idx)
	{
		BTreeNode<T, Compare> *child = (idx != positionOfFirstKey + n) ? C[idx % ndCapacity] : c_last;
		BTreeNode<T, Compare> *sibling = C[(idx - 1) % ndCapacity];

		// The last key from C[idx-1] goes up to the parent and key[idx-1]
		// from parent is inserted as the first key in C[idx]. Thus, the  loses
		// sibling one key and child gains one key

		// Moving all key in C[idx] one step ahead
		for (int i = child->positionOfFirstKey + child->n - 1; i >= child->positionOfFirstKey; --i)
			child->keys[(i + 1) % ndCapacity] = child->keys[i % ndCapacity];

		// If C[idx] is not a leaf, move all its child pointers one step ahead
		if (!child->leaf)
		{
			for (int i = child->positionOfFirstKey + child->n - 1; i >= positionOfFirstKey; --i)
				child->C[(i + 1) % ndCapacity] = child->C[i % ndCapacity];
		}

		// Setting child's first key equal to keys[idx-1] from the current node
		child->keys[child->positionOfFirstKey % ndCapacity] = keys[(idx - 1) % ndCapacity];

		// Moving sibling's last child as C[idx]'s first child
		if (!leaf) {
			child->C[child->positionOfFirstKey] = sibling->c_last;
			
			sibling->c_last = sibling->C[(sibling->positionOfFirstKey + sibling->n - 1) % ndCapacity]; // sibling's last child changes!
		}

		// Moving the key from the sibling to the parent
		// This reduces the number of keys in the sibling
		keys[(idx - 1) % ndCapacity] = sibling->keys[(sibling->positionOfFirstKey + sibling->n - 1) % ndCapacity];

		child->n += 1;
		sibling->n -= 1;

		return;
	}

	// A function to borrow a key from the C[idx+1] and place
	// it in C[idx]
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::borrowFromNext(int idx)
	{
		BTreeNode<T, Compare> *child = C[idx % ndCapacity];
		BTreeNode<T, Compare> *sibling = (idx + 1 != positionOfFirstKey + n) ? C[(idx + 1) % ndCapacity] : c_last;

		// keys[idx] is inserted as the last key in C[idx]
		child->keys[(child->positionOfFirstKey + child->n) % ndCapacity] = keys[idx % ndCapacity];

		// Sibling's first child is inserted as the last child
		// into C[idx]
		if (!(child->leaf)) {
			child->C[(child->positionOfFirstKey + child->n) % ndCapacity] = child->c_last; // Move the (old) last child to real child array
			child->c_last = sibling->C[positionOfFirstKey];
		}

		//The first key from sibling is inserted into keys[idx]
		keys[idx % ndCapacity] = sibling->keys[sibling->positionOfFirstKey];

		// Moving all keys in sibling one step behind
		for (int i = 1 + sibling->positionOfFirstKey; i < sibling->n + sibling->positionOfFirstKey; ++i)
			sibling->keys[(i - 1) % ndCapacity] = sibling->keys[i % ndCapacity];

		// Moving the child pointers one step behind
		if (!sibling->leaf)
		{
			for (int i = sibling->positionOfFirstKey + 1; i < sibling->positionOfFirstKey + sibling->n; ++i)
				sibling->C[(i - 1) % ndCapacity] = sibling->C[i % ndCapacity];
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
	bool BTreeNode<T, Compare>::merge(int idx)
	{
		// flag says that right child is the last one
		bool flag = ((idx + 1 == n + positionOfFirstKey) ? true : false);

		BTreeNode<T, Compare> *child = C[idx % ndCapacity];
		BTreeNode<T, Compare> *sibling = flag ? c_last : C[(idx + 1) % ndCapacity];

		// Pulling a key from the current node and inserting it into (t-1)th
		// position of C[idx]
		child->keys[(child->positionOfFirstKey + t - 1) % ndCapacity] = keys[idx % ndCapacity];

		// Copying the keys from C[idx+1] to C[idx] at the end
		for (int i = 0; i < sibling->n; ++i)
			child->keys[(i + t + child->positionOfFirstKey) % ndCapacity] = sibling->keys[(i + sibling->positionOfFirstKey) % ndCapacity];

		// Copying the child pointers from C[idx+1] to C[idx]
		if (!child->leaf)
		{
			child->C[(child->positionOfFirstKey + t - 1) % ndCapacity] = child->c_last; // Move last child to its real position
			
			for (int i = 0; i < sibling->n; ++i)
				child->C[(i + t + child->positionOfFirstKey) % ndCapacity] = sibling->C[(i + sibling->positionOfFirstKey) % ndCapacity];
			
			child->c_last = sibling->c_last;
		}

		// Moving all keys after idx in the current node one step before -
		// to fill the gap created by moving keys[idx] to C[idx]
		for (int i = idx + 1; i < n + positionOfFirstKey; ++i)
			keys[(i - 1) % ndCapacity] = keys[i % ndCapacity];

		// Moving the child pointers after (idx+1) in the current node one
		// step before
		for (int i = idx + 2; i < n + positionOfFirstKey; ++i) // ბოლო დარჩება სადაც იყო
			C[(i - 1) % ndCapacity] = C[i % ndCapacity];

		// Updating the key count of child and the current node
		child->n += sibling->n + 1;
		n--;

		// Freeing the memory occupied by sibling
		delete(sibling);
		return flag;
	}

	// A utility function to insert a new key in this node
	// ჩვენი დაშვებაა, რომ კვანძი არ არის სავსე ამ ფუნქციის გამოძახებისას
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::insertNonFull(T k) // Optimized!
	{
		// ინდექსი, რომლიდანაც დაწყებული წაძვრა უნდა მოხდეს (ანუ სადაც უნდა ჩაისვას ელემენტი)
		int i = find_ind_inNode(this, k, positionOfFirstKey, positionOfFirstKey + n - 1);

		// თუ მიმდინარე კვანძი ფოთოლია
		if (leaf == true)
		{
			int st = positionOfFirstKey;
			int fin = positionOfFirstKey + n - 1;

			if (i - st >= fin - i) // თუ ბოლოსთან უფრო ახლოსაა (ან შუაშია)
			{
				// სადაც უნდა ყოფილიყო, თუ უფრო პატარა გასაღები დახვდა, მაშინ  მარჯვნივ გაიწევს ინდექსი 
				if (cmp(keys[(i + ndCapacity) % ndCapacity], k)) ++i;

				// ზოგიერთი გასაღების გაწევა, მარჯვენა ბოლოდან დაწყებული, რომ ადგილი გავათავისუფლოთ
				while (i <= fin)
				{
					keys[(fin + 1 + ndCapacity) % ndCapacity] = keys[(fin + ndCapacity) % ndCapacity];
					--fin;
				}
				keys[(i + ndCapacity) % ndCapacity] = k; // გასაღების ჩასმა
			}
			else // თუ თავთან უფრო ახლოა, მაშინ ელემენტების ნაწილია მარცხნივ გადმოსაწევი
			{
				// პირველი გასაღების პოზიცია მარცხნივ გაიწევს
				positionOfFirstKey = (st - 1 + ndCapacity) % ndCapacity;

				// გასაღები სადაც უნდა ყოფილიყო, თუ უფრო დიდი გასაღები დახვდა, მაშინ  მარცხნივ გაიწევს ინდექსი 
				if (cmp(k, keys[(i + ndCapacity) % ndCapacity])) --i;
				while (st <= i)
				{
					keys[(st - 1 + ndCapacity) % ndCapacity] = keys[st % ndCapacity];
					++st;
				}
				keys[(i + ndCapacity) % ndCapacity] = k; // გასაღების ჩასმა
			}
			n = n + 1; // ზომის გაზრდა
		}
		else // თუ ეს კვანძი არ არის ფოთოლი
		{
			// გასაღები სადაც უნდა ყოფილიყო, თუ უფრო დიდი გასაღები დახვდა, მაშინ  მარცხნივ გაიწევს ინდექსი 
			if (k < keys[i % ndCapacity]) --i;

			// ბოლოში ხდება ჩასმა თუ არა?
			BTreeNode<T, Compare>* childToInsertInto;
			if (i + 1 == positionOfFirstKey + n)
				childToInsertInto = c_last;
			else 
				childToInsertInto = C[(i + 1) % ndCapacity];

			// სადაც ჩასმა ხდება სავსეა თუ არა?
			if (childToInsertInto->n == 2 * t - 1)
			{
				// თუ სავსეა, მაშინ გაყავი
				i = splitChild(i + 1, childToInsertInto);

				childToInsertInto = C[(i + ndCapacity) % ndCapacity]; // სპლიტი შეცვლიდა ამიტომაც ვარესეტებთ!!!!!

				// მარჯვენა შვილში თუ არის კანდიდატი
				if (cmp(keys[(i + ndCapacity) % ndCapacity], k)) // keys[i + 1] < k
				{
					// თუ ბოლოში ხდება ჩამატება მაშინ მარჯვენა შვილი არის სათადარიგო კვანძში
					if (i + 1 == positionOfFirstKey + n)
						childToInsertInto = c_last;
					else
						childToInsertInto = C[(i + 1 + ndCapacity) % ndCapacity];
				}
			}
			childToInsertInto->insertNonFull(k);
		}
	}

	// A utility function to split the child y of this node
	// შენიშვნა: y კვანძი უნდა იყოს სავსე ამ ფუნქციის გამოძახებისას
	// აქ i არ არის რეალური ინდექსი (i % capacity არის რეალური)
	template<typename T, typename Compare>
	int BTreeNode<T, Compare>::splitChild(int i, BTreeNode<T, Compare> *y)
	{
		// ახალი კვანძი რომელიც შეინახავს y-ის პირველ (t-1) გასაღებს
		BTreeNode<T, Compare> *z = new BTreeNode<T, Compare>(y->t, y->leaf);
		z->positionOfFirstKey = 0;
		z->n = t - 1;

		// დააკოპირე პირველი (t-1) გასაღები y-დან z-ში
		for (int j = 0; j < t - 1; j++)
			z->keys[j] = y->keys[(j + y->positionOfFirstKey) % y->ndCapacity];

		if (y->leaf == false) // თუ, ფოთოლი არაა, მაშინ დააკოპირე პირველი t შვილი y-დან z-ში
		{
			for (int j = 0; j < t - 1; j++)
				z->C[j] = y->C[(j + y->positionOfFirstKey) % y->ndCapacity];

			z->c_last = y->C[(y->positionOfFirstKey + t - 1) % y->ndCapacity]; // სათადარიგოში გადმოტანა
		}

		y->positionOfFirstKey = (y->positionOfFirstKey + t) % y->ndCapacity;
		y->n = t - 1; // y-ის ზომის შემცირება

		// თუ ფესვი იზრდება, მაშინ ბოლო (სათადარიგო) შვილი იქნება y!
		if (n == 0)
			c_last = y;
		else {
			int st = positionOfFirstKey;
			int fin = positionOfFirstKey + n - 1;

			if (i - st >= fin - i) // თუ ბოლოსთან უფრო ახლოსაა (ან შუაშია)
			{
				// ზოგიერთი გასაღების გაწევა, მარჯვენა ბოლოდან დაწყებული, რომ ადგილი გავათავისუფლოთ
				while (i <= fin)
				{
					keys[(fin + 1 + ndCapacity) % ndCapacity] = keys[(fin + ndCapacity) % ndCapacity];
					C[(fin + 1 + ndCapacity) % ndCapacity] = C[(fin + ndCapacity) % ndCapacity];
					--fin;
				}
			}
			else // თუ თავთან უფრო ახლოა, მაშინ ელემენტების ნაწილია მარცხნივ გადმოსაწევი
			{
				// პირველი გასაღების პოზიცია მარცხნივ გაიწევს
				positionOfFirstKey = (st - 1 + ndCapacity) % ndCapacity;
				
				--i; // ინდექსი მარცხნივ გავწიოთ პატარა ელემენტზე!

				while (st <= i)
				{
					keys[(st - 1 + ndCapacity) % ndCapacity] = keys[st % ndCapacity];
					C[(st - 1 + ndCapacity) % ndCapacity] = C[st % ndCapacity];
					++st;
				}
			}
		}

		// მარცხენა შვილი იქნება z
		C[(i + ndCapacity) % ndCapacity] = z;
		
		// Copy the middle key of y to this node
		keys[(i + ndCapacity) % ndCapacity] = y->keys[(y->positionOfFirstKey - 1 + ndCapacity) % ndCapacity];

		// Increment count of keys in this node
		n = n + 1;

		return i;
	}

	// Function to traverse all nodes in a subtree rooted with this node (needs to specify the printableValue(..) function)
	template<typename T, typename Compare>
	string BTreeNode<T, Compare>::traverse()
	{
		stringstream result; 
		result << " [";
		// There are n keys and n+1 children, travers through n keys
		// and first n children
		int i;
		for (i = positionOfFirstKey; i < positionOfFirstKey + n; i++)
		{
			// If this is not leaf, then before printing key[i],
			// traverse the subtree rooted with child C[i].
			if (leaf == false)
				result << C[(i + ndCapacity) % ndCapacity]->traverse();
			result << keys[(i + ndCapacity) % ndCapacity] << " ";
		}

		// Print the subtree rooted with last child
		if (leaf == false) // traverse last child 
			result << c_last->traverse();
		result << "] ";
		return result.str();
	}

	// Function to search key k in subtree rooted with this node
	template<typename T, typename Compare>
	BTreeNode<T, Compare> *BTreeNode<T, Compare>::search(T k)
	{
		// Old code (obsolete)
		//int i = positionOfFirstKey;
		//while (i < n + positionOfFirstKey && cmp(keys[i % ndCapacity], k)) // keys[i] < k
		//	i++;

		// Find the first key greater than or equal to k
		/*int left = positionOfFirstKey, right = positionOfFirstKey + n - 1, middle, i = positionOfFirstKey;
		while (left <= right)
		{
			middle = (left + right) / 2;
			if (k == keys[middle % ndCapacity])
			{
				i = middle;
				right = middle - 1;
				break;
			}
			else
				if (k < keys[middle % ndCapacity]) {
					i = middle;
					right = middle - 1;
				}
				else {
					left = middle + 1;
					i = left;
				}
		}*/


		// ინდექსი, რომლიდანაც დაწყებული წაძვრა უნდა მოხდეს (ანუ სადაც უნდა ჩაისვას ელემენტი)
		int i = find_ind_inNode(this, k, positionOfFirstKey, positionOfFirstKey + n - 1);

		if (cmp(keys[i % ndCapacity], k)) ++i;

		/*int left = positionOfFirstKey, right = positionOfFirstKey + n - 1, middle, i = 0;
		while (left <= right)
		{
			middle = (left + right) / 2;
			if (k == keys[middle % ndCapacity])
			{
				i = middle;
				right = middle - 1;
			}
			else
				if (k < keys[middle % ndCapacity]) {
					i = middle;
					right = middle - 1;
				}
				else {
					left = middle + 1;
					i = left;
				}
		}*/

		// თუ i არ არის საზღვრებს იქით და i-ური გასაღები k-ს ტოლია
		if (keys[i % ndCapacity] == k) // keys[i] == k!
			return this;

		// If key is not found here and this is a leaf node
		if (leaf == true)
			return NULL;

		// Go to the appropriate child
		if (i == n + positionOfFirstKey) // Should look in last child!
			return c_last->search(k);
		else
			return C[i % ndCapacity]->search(k);
	}
}
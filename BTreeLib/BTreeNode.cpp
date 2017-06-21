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
		//pairs = (NodePair<T, Compare>**)malloc(sizeof(NodePair<T, Compare>*) * (ndCapacity)); // new T[2 * t - 1];
		pairs = new NodePair<T, Compare>*[2 * t - 1]; // new T[2 * t - 1];

		// Initialize the number of keys as 0
		n = 0;

		cmp = _cmp;
	}

	template<typename T, typename Compare>
	BTreeNode<T, Compare>::~BTreeNode()
	{
		delete pairs; pairs = NULL;
	}

	// A utility function that returns the index of the first key that is
	// greater than or equal to k
	template<typename T, typename Compare>
	int BTreeNode<T, Compare>::findKey(T k)
	{
		return find_ind_inNode(this, k, positionOfFirstKey, positionOfFirstKey + n - 1);
	}

	// A function to remove the key k from the sub-tree rooted with this node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::remove(T k)
	{
		int idx = findKey(k);

		if (cmp(pairs[idx % ndCapacity]->getKey(), k)) idx++; // ინდექსის გასწორება!

		// The key to be removed is present in this node
		if (idx < positionOfFirstKey + n && pairs[idx % ndCapacity]->getKey() == k) // keys[idx] == k!
		{
			// If the node is a leaf node - removeFromLeaf is called
			// Otherwise, removeFromNonLeaf function is called
			if (leaf)
				removeFromLeaf(idx);
			else
				removeFromNonLeaf(idx);
		}
		else // მიმდინარე კვანძში თუ ვერ მოიძებნა
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
			bool flag = ((idx >= n - 1 + positionOfFirstKey) ? true : false); // ბოლოა ან ბოლოს შემდეგი

			BTreeNode<T, Compare>* idxChild = (idx >= n + positionOfFirstKey) ? c_last : pairs[idx % ndCapacity]->getChild(); // თუ ბოლო შვილია 

			// If the child where the key is supposed to exist has less that t keys,
			// we fill that child
			int newIdx = idx;
			if (idxChild->n < t) {
				int i = fill(idx);
				if (i != -1) {
					newIdx = i;
				}
			}

			// If the last child has been merged, it must have merged with the previous
			// child and so we recurse on the (idx-1)th child. Else, we recurse on the
			// (idx)th child which now has atleast t keys
			if (flag && idx > n - 1 + positionOfFirstKey)
				c_last->remove(k);
			else
				pairs[newIdx % ndCapacity]->getChild()->remove(k);
		}
		return;
	}

	// A function to remove the idx-th key from this node - which is a leaf node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::removeFromLeaf(int i)
	{
		int st = positionOfFirstKey;
		int fin = positionOfFirstKey + n - 1;

		if (i - st >= fin - i) // თუ ბოლოსთან უფრო ახლოსაა (ან შუაშია)
		{
			// ზოგიერთი გასაღების გაწევა, მარჯვენა ბოლოდან დაწყებული, რომ ადგილი დაიფაროს
			while (i < fin)
			{
				pairs[i % ndCapacity] = pairs[(i + 1) % ndCapacity];
				++i;
			}
		}
		else // თუ თავთან უფრო ახლოა, მაშინ ელემენტების ნაწილია მარცხნივ გადმოსაწევი
		{
			// პირველი გასაღების პოზიცია მარცხნივ გაიწევს
			positionOfFirstKey = (st + 1) % ndCapacity;

			while (st < i)
			{
				pairs[i % ndCapacity] = pairs[(i - 1 + ndCapacity) % ndCapacity];
				--i;
			}
		}

		n--; // Reduce the count of keys
		return;
	}

	// A function to remove the idx-th key from this node - which is a non-leaf node
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::removeFromNonLeaf(int idx)
	{
		T k = pairs[idx % ndCapacity]->getKey();

		// წინასწარ განსაზღვრე მარჯვენა შვილი არის თუ არა ბოლო
		BTreeNode<T, Compare>* rightChild = (idx + 1 >= positionOfFirstKey + n) ? c_last : pairs[(idx + 1) % ndCapacity]->getChild();

		// If the child that precedes k (C[idx]) has atleast t keys,
		// find the predecessor 'pred' of k in the subtree rooted at
		// C[idx]. Replace k by pred. Recursively delete pred
		// in C[idx]
		if (pairs[idx % ndCapacity]->getChild()->n >= t)
		{
			T pred = getPred(idx);
			pairs[idx % ndCapacity]->setKey(pred);
			pairs[idx % ndCapacity]->getChild()->remove(pred);
		}

		// If the child C[idx] has less that t keys, examine C[idx+1].
		// If C[idx+1] has atleast t keys, find the successor 'succ' of k in
		// the subtree rooted at C[idx+1]
		// Replace k by succ
		// Recursively delete succ in C[idx+1]
		else if (rightChild->n >= t)
		{
			T succ = getSucc(idx);
			pairs[idx % ndCapacity]->setKey(succ);
			rightChild->remove(succ);
		}

		// If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
		// into C[idx]
		// Now C[idx] contains 2t-1 keys
		// Free C[idx+1] and recursively delete k from C[idx]
		else
		{
			// თუ ბოლოები შეერთდა
			idx = merge(idx);
			if (idx == -1)
				c_last->remove(k);
			else
				pairs[idx % ndCapacity]->getChild()->remove(k);
		}
		return;
	}

	// A function to get predecessor of keys[idx]
	template<typename T, typename Compare>
	T BTreeNode<T, Compare>::getPred(int idx)
	{
		// Keep moving to the right most node until we reach a leaf
		BTreeNode<T, Compare> *cur = pairs[idx % ndCapacity]->getChild();
		while (!cur->leaf)
			cur = cur->c_last;

		// Return the last key of the leaf
		return cur->pairs[(cur->positionOfFirstKey + cur->n - 1) % ndCapacity]->getKey();
	}

	template<typename T, typename Compare>
	T BTreeNode<T, Compare>::getSucc(int idx)
	{
		// Keep moving the left most node starting from C[idx+1] until we reach a leaf
		BTreeNode<T, Compare> *cur = (idx + 1 != positionOfFirstKey + n) ? pairs[(idx + 1) % ndCapacity]->getChild() : c_last;
		while (!cur->leaf)
			cur = cur->pairs[cur->positionOfFirstKey]->getChild();

		// Return the first key of the leaf
		return cur->pairs[cur->positionOfFirstKey]->getKey();
	}

	// A function to fill child C[idx] which has less than t-1 keys
	template<typename T, typename Compare>
	int BTreeNode<T, Compare>::fill(int idx)
	{
		bool flag = ((idx + 1 >= n + positionOfFirstKey) ? true : false);
		// TODO speed up!
		BTreeNode<T, Compare>* rightChild = flag ? c_last : pairs[(idx + 1) % ndCapacity]->getChild();

		// If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
		// from that child
		if (idx != positionOfFirstKey && pairs[(idx - 1) % ndCapacity]->getChild()->n >= t)
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
			if (idx != positionOfFirstKey + n)
				idx = merge(idx);
			else
				idx = merge(idx - 1);

			// merge თავად ასწორებს ბოლო შვილს!
		}
		return idx;
	}

	// A function to borrow a key from C[idx-1] and insert it
	// into C[idx]
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::borrowFromPrev(int idx)
	{
		BTreeNode<T, Compare> *child = (idx != positionOfFirstKey + n) ? pairs[idx % ndCapacity]->getChild() : c_last;
		BTreeNode<T, Compare> *sibling = pairs[(idx - 1) % ndCapacity]->getChild();

		// The last key from C[idx-1] goes up to the parent and key[idx-1]
		// from parent is inserted as the first key in C[idx]. Thus, the  loses
		// sibling one key and child gains one key

		// პოზიციის გათავისუფლება პირველი ელემენტისთვის
		child->positionOfFirstKey = (child->positionOfFirstKey - 1 + ndCapacity) % ndCapacity;

		// Setting child's first key equal to keys[idx-1] from the current node
		child->pairs[child->positionOfFirstKey % ndCapacity]->setKey(pairs[(idx - 1 + ndCapacity) % ndCapacity]->getKey());

		// Moving sibling's last child as C[idx]'s first child
		if (!leaf)
		{
			child->pairs[child->positionOfFirstKey]->setChild(sibling->c_last);

			// sibling's last child changes!
			sibling->c_last = sibling->pairs[(sibling->positionOfFirstKey + sibling->n - 1 + ndCapacity) % ndCapacity]->getChild();
		}

		// Moving the key from the sibling to the parent
		// This reduces the number of keys in the sibling
		pairs[(idx - 1) % ndCapacity]->setKey(
			sibling->pairs[(sibling->positionOfFirstKey + sibling->n - 1) % ndCapacity]->getKey());

		child->n += 1;
		sibling->n -= 1;
		return;
	}

	// A function to borrow a key from the C[idx+1] and place it in C[idx]
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::borrowFromNext(int idx)
	{
		BTreeNode<T, Compare> *child = pairs[idx % ndCapacity]->getChild();
		BTreeNode<T, Compare> *sibling = (idx + 1 != positionOfFirstKey + n) ? pairs[(idx + 1) % ndCapacity]->getChild() : c_last;

		// keys[idx] is inserted as the last key in C[idx]
		child->pairs[(child->positionOfFirstKey + child->n) % ndCapacity]->setKey(pairs[idx % ndCapacity]->getKey());

		// Sibling's first child is inserted as the last child
		// into C[idx]
		if (!(child->leaf)) 
		{
			// Move the (old) last child to real child array
			child->pairs[(child->positionOfFirstKey + child->n) % ndCapacity]->setChild(child->c_last);
			child->c_last = sibling->pairs[sibling->positionOfFirstKey]->getChild();
		}

		// The first key from sibling is inserted into keys[idx]
		pairs[idx % ndCapacity]->setKey(sibling->pairs[sibling->positionOfFirstKey]->getKey());

		// Move siblings start one step forward
		sibling->positionOfFirstKey = (sibling->positionOfFirstKey + 1) % ndCapacity;

		// Increasing and decreasing the key count of C[idx] and C[idx+1]
		// respectively
		child->n += 1;
		sibling->n -= 1;
		return;
	}

	// A function to merge C[idx] with C[idx+1]
	// C[idx+1] is freed after merging
	template<typename T, typename Compare>
	int BTreeNode<T, Compare>::merge(int idx)
	{
		// flag says that right child is the last one
		bool flag = ((idx + 1 >= n + positionOfFirstKey) ? true : false);

		BTreeNode<T, Compare> *child = pairs[idx % ndCapacity]->getChild();
		BTreeNode<T, Compare> *sibling = flag ? c_last : pairs[(idx + 1) % ndCapacity]->getChild();

		// Pulling a key from the current node and inserting it into (t-1)th
		// position of C[idx]
		child->pairs[(child->positionOfFirstKey + t - 1) % ndCapacity]->setKey(
			pairs[idx % ndCapacity]->getKey());

		// Copying the keys from C[idx+1] to C[idx] at the end
		for (int i = 0; i < sibling->n; ++i)
			child->pairs[(i + t + child->positionOfFirstKey) % ndCapacity]->setKey(
				sibling->pairs[(i + sibling->positionOfFirstKey) % ndCapacity]->getKey());

		// Copying the child pointers from C[idx+1] to C[idx]
		if (!child->leaf)
		{
			child->pairs[(child->positionOfFirstKey + t - 1) % ndCapacity]->setChild(child->c_last); // Move last child to its real position

			for (int i = 0; i < sibling->n; ++i)
				child->pairs[(i + t + child->positionOfFirstKey) % ndCapacity]->setChild(
					sibling->pairs[(i + sibling->positionOfFirstKey) % ndCapacity]->getChild());

			child->c_last = sibling->c_last;
		}

		int st = positionOfFirstKey;
		int fin = positionOfFirstKey + n - 1;
		int i = idx;
		if (i - st >= fin - i) // თუ ბოლოსთან უფრო ახლოსაა (ან შუაშია)
		{
			// Save to restore after loop!
			BTreeNode<T, Compare>* backupTmp = pairs[i % ndCapacity]->getChild();

			// ზოგიერთი გასაღების გაწევა, მარჯვენა ბოლოდან დაწყებული, რომ ადგილი დაიფაროს
			while (i < fin)
			{
				pairs[i % ndCapacity] = pairs[(i + 1) % ndCapacity];

				// (idx + 1)-დან დაწყებული
				/*if (idx < i)
					C[i % ndCapacity] = C[(i + 1) % ndCapacity];*/

				++i;
			}

			pairs[idx]->setChild(backupTmp); // Restore overwritten child!

			if (flag)
				c_last = child;
		}
		else // თუ თავთან უფრო ახლოა, მაშინ ელემენტების ნაწილია გადასაწევი
		{
			// პირველი გასაღების პოზიცია გაიწევს
			positionOfFirstKey = (positionOfFirstKey + 1) % ndCapacity;

			// თუ მარჯვენა შვილი ბოლო არაა, მაშინ გადატანა საჭიროა
			if (flag)
				c_last = child;
			else
				pairs[(i + 1) % ndCapacity]->setChild(
					pairs[i % ndCapacity]->getChild());

			while (st < i)
			{
				/*keys[i % ndCapacity] = keys[(i - 1 + ndCapacity) % ndCapacity];
				C[i % ndCapacity] = C[(i - 1 + ndCapacity) % ndCapacity];*/
				pairs[i % ndCapacity] = pairs[(i - 1 + ndCapacity) % ndCapacity];
				--i;
			}

			idx++; // Move index to right
		}

		// Updating the key count of child and the current node
		child->n += sibling->n + 1;
		n--;

		// Freeing the memory occupied by sibling
		delete(sibling);
		return flag ? -1 : idx;
	}

	// A utility function to insert a new key in this node
	// ჩვენი დაშვებაა, რომ კვანძი არ არის სავსე ამ ფუნქციის გამოძახებისას
	template<typename T, typename Compare>
	void BTreeNode<T, Compare>::insertNonFull(T k)
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
				if (cmp(pairs[(i + ndCapacity) % ndCapacity]->getKey(), k)) ++i;

				// ზოგიერთი გასაღების გაწევა, მარჯვენა ბოლოდან დაწყებული, რომ ადგილი გავათავისუფლოთ
				while (i <= fin)
				{
					pairs[(fin + 1 + ndCapacity) % ndCapacity] = pairs[(fin + ndCapacity) % ndCapacity];
					--fin;
				}
				pairs[(i + ndCapacity) % ndCapacity] = new NodePair<T, Compare>(k); // გასაღების ჩასმა
			}
			else // თუ თავთან უფრო ახლოა, მაშინ ელემენტების ნაწილია მარცხნივ გადმოსაწევი
			{
				// პირველი გასაღების პოზიცია მარცხნივ გაიწევს
				positionOfFirstKey = (st - 1 + ndCapacity) % ndCapacity;

				// გასაღები სადაც უნდა ყოფილიყო, თუ უფრო დიდი გასაღები დახვდა, მაშინ  მარცხნივ გაიწევს ინდექსი 
				if (cmp(k, pairs[(i + ndCapacity) % ndCapacity]->getKey())) --i;
				while (st <= i)
				{
					pairs[(st - 1 + ndCapacity) % ndCapacity] = pairs[st % ndCapacity];
					++st;
				}
				pairs[(i + ndCapacity) % ndCapacity] = new NodePair<T, Compare>(k); // გასაღების ჩასმა
			}
			n = n + 1; // ზომის გაზრდა
		}
		else // თუ ეს კვანძი არ არის ფოთოლი
		{
			// გასაღები სადაც უნდა ყოფილიყო, თუ უფრო დიდი გასაღები დახვდა, მაშინ  მარცხნივ გაიწევს ინდექსი 
			if (cmp(k, pairs[i % ndCapacity]->getKey())) --i;

			// ბოლოში ხდება ჩასმა თუ არა?
			BTreeNode<T, Compare>* childToInsertInto;
			if (i + 1 == positionOfFirstKey + n)
				childToInsertInto = c_last;
			else
				childToInsertInto = pairs[(i + 1) % ndCapacity]->getChild();

			// სადაც ჩასმა ხდება სავსეა თუ არა?
			if (childToInsertInto->n == 2 * t - 1)
			{
				// თუ სავსეა, მაშინ გაყავი
				i = splitChild(i + 1, childToInsertInto);

				childToInsertInto = pairs[(i + ndCapacity) % ndCapacity]->getChild(); // სპლიტი შეცვლიდა ამიტომაც ვარესეტებთ!!!!!

				// მარჯვენა შვილში თუ არის კანდიდატი
				if (cmp(pairs[(i + ndCapacity) % ndCapacity]->getKey(), k)) // keys[i + 1] < k
				{
					// თუ ბოლოში ხდება ჩამატება მაშინ მარჯვენა შვილი არის სათადარიგო კვანძში
					if (i + 1 == positionOfFirstKey + n)
						childToInsertInto = c_last;
					else
						childToInsertInto = pairs[(i + 1 + ndCapacity) % ndCapacity]->getChild();
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

		// დააკოპირე პირველი (t-1) წყვილი y-დან z-ში
		for (int j = 0; j < t - 1; j++)
			z->pairs[j] = y->pairs[(j + y->positionOfFirstKey) % y->ndCapacity];

		if (y->leaf == false)
		{
			z->c_last = y->pairs[(y->positionOfFirstKey + t - 1) % y->ndCapacity]->getChild(); // სათადარიგოში გადმოტანა
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
					pairs[(fin + 1 + ndCapacity) % ndCapacity] = pairs[(fin + ndCapacity) % ndCapacity]; // Speed up!					
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
					pairs[(st - 1 + ndCapacity) % ndCapacity] = pairs[st % ndCapacity]; // Speed up!
					++st;
				}
			}
		}

		// მარცხენა შვილი იქნება z && Copy the middle key of y to this node
		pairs[(i + ndCapacity) % ndCapacity] = new NodePair<T, Compare>(y->pairs[(y->positionOfFirstKey - 1 + ndCapacity) % ndCapacity]->getKey(), z);

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
				result << pairs[(i + ndCapacity) % ndCapacity]->getChild()->traverse();
			result << pairs[(i + ndCapacity) % ndCapacity]->getKey() << " ";
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
		// ინდექსი, რომლიდანაც დაწყებული წაძვრა უნდა მოხდეს (ანუ სადაც უნდა ჩაისვას ელემენტი)
		int i = find_ind_inNode(this, k, positionOfFirstKey, positionOfFirstKey + n - 1);

		if (cmp(pairs[i % ndCapacity]->getKey(), k)) ++i;

		// თუ i არ არის საზღვრებს იქით და i-ური გასაღები k-ს ტოლია
		if (i != n + positionOfFirstKey && pairs[i % ndCapacity]->getKey() == k) // keys[i] == k!
			return this;

		// If key is not found here and this is a leaf node
		if (leaf == true)
			return NULL;

		// Go to the appropriate child
		if (i == n + positionOfFirstKey) // Should look in last child!
			return c_last->search(k);
		else
			return pairs[i % ndCapacity]->getChild()->search(k);
	}
}
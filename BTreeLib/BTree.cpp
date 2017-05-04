#include "BTree.h"

namespace BTreeLib
{
	template<typename T, typename Compare>
	void BTree<T, Compare>::insert(T k)
	{
		// თუ ხე ცარიელია
		if (root == NULL)
		{
			// მეხსიერების გამოყოფა ფესვისთვის
			root = new BTreeNode<T, Compare>(t, true);
			root->keys[root->positionOfFirstKey] = k;  // გასაღების ჩასმა (0 ინდექსზე)
			root->n = 1;  // გასაღებების რაოდენობის განახლება
		}
		else // თუ ხე არ არის ცარიელი
		{
			// თუ ფესვი სავსეა, მაშინ ხე იზრდება სიმაღლეში
			if (root->n == 2 * t - 1)
			{
				// გამოვყოფთ მეხსიერებას ახალი ფესვისთვის
				BTreeNode<T, Compare> *s = new BTreeNode<T, Compare>(t, false);

				// გახადე ძველი ფესვი ახალი ფესვის, s-ის შვილი
				s->C[0] = root;

				// გაყავი ძველი ფესვი და გადაიტანე 1 გასაღები ახალ ფესვში
				s->splitChild(0, root);

				// ახალ ფესვს ახლა ორი შვილი ჰყავს. გადაწყვიტე 
				// ამ ორიდან რომელს ექნება ახალი გასაღები
				int i = s->positionOfFirstKey;
				if (cmp(s->keys[i], k)) // s->keys[0] < k
					i++;
				s->C[i]->insertNonFull(k);

				// ვცვლით ფესვს ახლით
				root = s;
			}
			else  // თუ ფესვი არ არის სავსე, მაშინ დაუძახე insertNonFull ფუნქციას ფესვისთვის
				root->insertNonFull(k);
		}
	}

	template<typename T, typename Compare>
	void BTree<T, Compare>::remove(T k)
	{
		if (!root)
		{
			cout << "The tree is empty\n";
			return;
		}

		// Call the remove function for root
		root->remove(k);

		// If the root node has 0 keys, make its first child as the new root
		//  if it has a child, otherwise set root as NULL
		if (root->n == 0)
		{
			BTreeNode<T, Compare> *tmp = root;
			if (root->leaf)
				root = NULL;
			else
				root = root->C[0];

			// Free the old root
			delete tmp;
		}
		return;
	}
}
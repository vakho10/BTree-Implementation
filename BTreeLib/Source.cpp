#include "../TestSuite/Utils.h"

#include <random>
#include <algorithm>
#include <functional>

#include <iostream>

#include "../BTreeLib/OldBTree.h"

#include "../BTreeLib/BTree.h"
#include "../BTreeLib/BTree.cpp" // To fix template class separation

#include "../BTreeLib/BTreeNode.h"
#include "../BTreeLib/BTreeNode.cpp" // To fix template class separation

using namespace std;
using namespace OldTree;
using namespace BTreeLib;

int main()
{
	OldBTree* t1 = new OldBTree(3);
	BTree<int>* t2 = new BTree<int>(3);

	//// Fill trees with test data before tests
	//for (int i = 2; i <= 10; i+=2)	{
	//	t2->insert(i);
	//}

	//t2->insert(-2);
	//t2->insert(2);
	//t2->insert(2);
	//t2->insert(12);

	//t2->traverse();
	//cout << endl;

	//auto node = t2->search(2);
	//if (node != NULL) {
	//	cout << node->findKey(3) << endl;
	//}
	//else {
	//	cout << "Not found!" << endl;
	//}


	// Fill trees with test data before tests
	// Shuffle array with different elements and test insertion
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(15, 20);
	int size = dis(gen);
	int *numbers = new int[size];
	for (int i = 1; i <= size; i++)
	{
		numbers[i - 1] = i;
	}
	std::random_shuffle(numbers, numbers + size);
	for (size_t i = 0; i < size; i++)
	{
		cout << numbers[i] << ", ";
	}
	cout << endl;

	for (int i = 0; i < size; i++)
	{
		t1->insert(numbers[i]);
		t2->insert(numbers[i]);
	}
	
	t1->traverse();
	cout << endl;
	t2->traverse();
	cout << endl;

	for (int i = 1; i <= size; i++)
	{
		int index1 = t1->search(i)->findKey(i);
		
		auto node = t2->search(i);
		
		if (node == NULL) {
			cout << "NULL!" << endl;
			break;
		}
		int index2 = node->findKey(i);
		cout << "Looking for: " << i;
		cout << index1 << ", " << index2 - node->getPositionOfFirstKey() << " => " << (index1 == index2) << endl;
	}




	//int k = 10;
	//int* keys = new int[3] { 12, 15, 16 };
	//int md, i = 0, st = 0, fin = 3;
	//i = fin;
	//while (st <= fin)
	//{
	//	md = (st + fin) / 2;
	//	if (k == keys[md])
	//	{
	//		fin = md - 1; // Goto left side and find another match
	//		i = fin;
	//	}
	//	else 
	//		if (k < keys[md]) // keys[middle] < k
	//		{
	//			fin = md - 1;
	//			i = fin;
	//		}
	//		else {
	//			st = md + 1;
	//		}
	//}
	//cout << i << endl;
}
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

	// Fill trees with test data before tests
	// Shuffle array with different elements and test insertion
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(110, 1110);
	//int size = 12;
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

	std::string result1 = t1->traverse();
	cout << result1 << endl;
	std::string result2 = t2->traverse();
	cout << result2 << endl;
	
	for (int i = 1; i <= size; i++)
	{
		int index1 = t1->search(i)->findKey(i);

		auto node = t2->search(i);
		int index2 = node->findKey(i);
		cout << "Looking for " << i << ": ";
		cout << index1 << ", " << index2 - node->getPositionOfFirstKey() << " => " << (index1 == index2) << endl;
	}
}
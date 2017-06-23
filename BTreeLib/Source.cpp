#include <random>
#include <algorithm>
#include <functional>
#include <chrono>
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
using namespace std::chrono;

time_point<high_resolution_clock> start, finish; // (C++11 Style) Chrono start and end time points.

int main()
{
	less<int> cmp = less<int>();

	start = high_resolution_clock::now();
	for (size_t i = 0; i < 10000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			if (i < j) {
				continue;
			}
		}
	}
	finish = high_resolution_clock::now();
	cout << "without cmp: " << duration_cast<milliseconds>(finish - start).count() << endl;

	start = high_resolution_clock::now();
	for (size_t i = 0; i < 10000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			if (cmp(i, j)) {
				continue;
			}
		}
	}
	finish = high_resolution_clock::now();
	cout << "without cmp: " << duration_cast<milliseconds>(finish - start).count() << endl;

	//OldBTree* t1 = new OldBTree(3);
	//BTree<int>* t2 = new BTree<int>(3);

	//// Fill trees with test data before tests
	//// Shuffle array with different elements and test insertion
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	//std::uniform_int_distribution<> dis(60, 60);
	////int size = 12;
	//int size = dis(gen);
	//int *numbers = new int[size];
	//for (int i = 1; i <= size; i++)
	//{
	//	numbers[i - 1] = i - (size / 2);
	//}
	////std::random_shuffle(numbers, numbers + size);
	///*for (size_t i = 0; i < size; i++)
	//{
	//	cout << numbers[i] << ", ";
	//}
	//cout << endl;*/

	//for (int i = 0; i < size; i++)
	//{
	//	t1->insert(numbers[i]);
	//	t2->insert(numbers[i]);
	//}

	//cout << t1->traverse() << endl;
	//cout << t2->traverse() << endl;

	//for (int i = 0; i < size; i++)
	//{
	//	auto node = t2->search(numbers[i]);
	//	int index2 = node->findKey(numbers[i]);
	//	int index1 = t1->search(numbers[i])->findKey(numbers[i]);

	//	// Correct index1
	//	index1 = (node->getPositionOfFirstKey() + index1) % node->getCapacity();

	//	cout << "Looking for " << numbers[i] << ": ";
	//	cout << index1 << ", " << index2 - node->getPositionOfFirstKey() << " => " << (index1 == (index2 - node->getPositionOfFirstKey())) << endl;
	//}

	//cout << t1->traverse() << endl;
	//cout << t2->traverse() << endl;

	//std::random_shuffle(numbers, numbers + size);

	//for (int i = 0; i < size; i++) 
	//{
	//	cout << "Deleting " << numbers[i] << "..." << endl;

	//	t1->remove(numbers[i]);
	//	t2->remove(numbers[i]);

	//	cout << t1->traverse() << endl;
	//	cout << t2->traverse() << endl;
	//	cout << (t1->traverse() == t2->traverse()) << endl;
	//}

	/*for (size_t i = 1000000; i <= 100000000; i *= 10)
	{
		int size = i;
		start = high_resolution_clock::now();
		NodePair<int>** pairs = (NodePair<int>**)malloc(sizeof(NodePair<int>*) * (size));
		finish = high_resolution_clock::now();
		cout << "malloc: " << size << " -> " << duration_cast<nanoseconds>(finish - start).count() << endl;
	}*/


}
// Using BTree implementation from here: http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
// Template class separation workarounds described here: https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp
// For time computation we're using chrono library: http://en.cppreference.com/w/cpp/chrono

#define TEN_MILLION 10000000

#include <iostream>
#include <string>
#include <chrono>
#include <set>
#include <cstdlib> 
#include <memory>
#include <random>
#include <algorithm> // for random shuffle

#include "../BTreeLib/BTree.h"
#include "../BTreeLib/BTree.cpp" // To fix template class separation

#include "../BTreeLib/BTreeNode.h"
#include "../BTreeLib/BTreeNode.cpp" // To fix template class separation

#include "../BTreeLib/RB.h"

#include "../BTreeLib/OldBTree.h"

using namespace std;
using namespace std::chrono;
using namespace BTreeLib;
using namespace OldTree;

// Globals:
time_point<high_resolution_clock> start, finish; // (C++11 Style) Chrono start and end time points.
int* integers;
int bestT = 64; // Has been checked previously
Node<int>** rbNodes;

// Prototypes:
void generateAndFillIntegers();
void testAndDetermineBestT();
void checkInsertionSearchDeletion();

// MAIN
int main()
{
	generateAndFillIntegers(); // Generate integers (shuffled)

	//testAndDetermineBestT(); // Testing t parameter (10, 100, 1000) for BTree (1m, 10m, 100m)	

	checkInsertionSearchDeletion();

	cout << "All the test have finished." << endl << endl;
	return 0;
}

void generateAndFillIntegers()
{
	integers = new int[TEN_MILLION];
	for (int i = TEN_MILLION - 1; i >= 0; i--) {
		integers[i] = i - (TEN_MILLION / 2);
	}
	//random_shuffle(integers, integers + TEN_MILLION);
}

void testAndDetermineBestT()
{
	int TsAndTimes[3][2]; // { t, insert_time }

	cout << "Testing T parameter for BTree (accoding to insertion speed only):" << endl;
	for (unsigned int t = 10, j = 0; t <= 1000; t *= 10, j++)
	{
		TsAndTimes[j][0] = t;
		TsAndTimes[j][1] = 0;

		BTree<int> tmpBTree(t);
		for (unsigned int i = 1; i <= 100; i *= 10)
		{
			int size = 1000000 * i;
			cout << "Size: " << size << endl;

			start = high_resolution_clock::now();
			for (unsigned int j = 0; j < size; j++)
				tmpBTree.insert(integers[j]);
			finish = high_resolution_clock::now();
			int millis = duration_cast<milliseconds>(finish - start).count();
			TsAndTimes[j][1] += millis;
		}
		cout << endl;
	}

	// deleteme
	for (size_t i = 0; i < 3; i++) cout << "Insert BTree -> t: " << TsAndTimes[i][0] << ", Milliseconds: " << TsAndTimes[i][1] << endl;

	// Determining the best t parameter (insert + delete times have to be lowest)
	int minimalIndex = 0;
	for (size_t i = 0; i < 3; i++)
		if (TsAndTimes[i][1] < TsAndTimes[minimalIndex][1])
			minimalIndex = i;

	bestT = TsAndTimes[minimalIndex][0];
	cout << "Optimal T is: " << bestT << endl;
}

void checkInsertionSearchDeletion()
{
	cout << "Testing insertions:" << endl;
	for (unsigned int size = 1000000; size <= TEN_MILLION; size *= 10)
	{
		rbNodes = new Node<int>*[size];
		for (size_t i = 0; i < size; i++)
			rbNodes[i] = new Node<int>(integers[i]);

		// BTree (old)
		OldTree::OldBTree oldBTree(bestT);
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			oldBTree.insert(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Insert OldBTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// BTree (insert)
		BTree<int> tmpBTree(bestT);
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpBTree.insert(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Insert BTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// RedBlackTree (insert)
		Tree<int> tmpRedBlackTree;
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++) {
			tmpRedBlackTree.RB_insert(rbNodes[j]);
		}
		finish = high_resolution_clock::now();
		cout << "Insert RedBlackTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// OldBTree (search)
		start = high_resolution_clock::now();
		for (size_t j = 0; j < size; j++)
			oldBTree.search(integers[j])->findKey(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Search BTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// BTree (search)
		start = high_resolution_clock::now();
		for (size_t j = 0; j < size; j++)
			tmpBTree.search(integers[j])->findKey(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Search BTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// RedBlackTree (search)
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpRedBlackTree.search(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Search RedBlackTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// OldBTree (delete)
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			oldBTree.remove(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Delete OldBTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// BTree (delete)
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpBTree.remove(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Delete BTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// RedBlackTree (delete)
		start = high_resolution_clock::now();
		for (size_t j = 0; j < size; j++) {
			tmpRedBlackTree.RB_delete(rbNodes[j]);
		}
		finish = high_resolution_clock::now();
		cout << "Delete RedBlackTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;
	}
}
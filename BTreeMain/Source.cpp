// Using BTree implementation from here: http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
// Template class separation workarounds described here: https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp
// For time computation we're using chrono library: http://en.cppreference.com/w/cpp/chrono

#include <iostream>
#include <string>
#include <chrono>
#include <Windows.h>
#include <set>

#include "BTree.h"
#include "BTree.cpp" // To fix template class separation

#include "BTreeNode.h"
#include "BTreeNode.cpp" // To fix template class separation

#include "RedBlackTreeNode.h"

#include "RedBlackTree.h"
#include "RedBlackTree.cpp" // To fix template class separation

using namespace std;
using namespace std::chrono;
using namespace BTreeLib;

// (C++11 Style) Chrono start and end time points.
time_point<high_resolution_clock> start, finish;
duration<double> elapsed_seconds;

int main()
{
	/*
		Testing t parameter (10, 100, 1000) for BTree (1m, 10m, 100m)
	*/
	cout << "Testing T parameter for BTree:" << endl;
	for (unsigned int t = 10; t <= 1000; t *= 10)
	{
		BTree<int> tmpBTree(t);
		for (unsigned int i = 1; i <= 100; i *= 10)
		{
			int size = 1000000 * i;
			cout << "Size: " << size << endl;

			// Fill it with random integers
			int* integers = new int[size];
			for (unsigned int j = 0; j < size; j++)
				integers[j] = -1000 + (rand() % (int)(1000 - (-1000) + 1)); // Ranges from -1000 to 1000

			start = high_resolution_clock::now();
			for (unsigned int j = 0; j < size; j++)
				tmpBTree.insert(integers[j]);
			finish = high_resolution_clock::now();
			elapsed_seconds = finish - start;
			cout << "Insert BTree -> t: " << t << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

			start = high_resolution_clock::now();
			for (unsigned int j = 0; j < size; j++)
				tmpBTree.remove(integers[j]);
			finish = high_resolution_clock::now();
			elapsed_seconds = finish - start;
			cout << "Delete BTree -> t: " << t << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;
		}
		cout << endl;
	}

	/*
		Running simple tests for BTree and RedBlackTree (int)
	*/
	//cout << "Testing insertions and deletions (for BTree and RedBlackTree):" << endl;
	//BTree<int> tMillions(3), tTenMillions(3), tHundredMillions(3);
	//for (unsigned int i = 1; i <= 100; i *= 10)
	//{
	//	int size = i * 1000000;

	//	// Fill it with random integers
	//	int* integers = new int[size];
	//	RedBlackTreeNode<int>** nodes = new RedBlackTreeNode<int>*[size];
	//	for (unsigned int j = 0; j < size; j++)
	//	{
	//		// Ranges from -1000 to 1000
	//		integers[j] = -1000 + (rand() % (int)(1000 - (-1000) + 1));
	//		nodes[j] = new RedBlackTreeNode<int>(integers[j]);
	//	}

	//	// BTree (Insert)
	//	BTree<int> tmpBTree(3);
	//	start = high_resolution_clock::now();
	//	for (unsigned int j = 0; j < size; j++)
	//		tmpBTree.insert(integers[j]);
	//	finish = high_resolution_clock::now();
	//	duration<double> elapsed_seconds = finish - start;
	//	cout << "Insert BTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

	//	// RedBlackTree (Insert)
	//	RedBlackTree<int> tmpRedBlackTree;
	//	start = high_resolution_clock::now();
	//	for (unsigned int j = 0; j < size; j++)
	//	{
	//		tmpRedBlackTree.insert(nodes[j]);
	//	}
	//	finish = high_resolution_clock::now();
	//	elapsed_seconds = finish - start;
	//	cout << "Insert RedBlackTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

	//	// BTree (Remove)
	//	/*start = high_resolution_clock::now();
	//	for (unsigned int j = 0; j < size; j++)
	//		tmpBTree.remove(integers[j]);
	//	finish = high_resolution_clock::now();
	//	elapsed_seconds = finish - start;
	//	cout << "Remove BTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;*/

	//	// RedBlackTree (Remove)
	//	/*start = high_resolution_clock::now();
	//	for (unsigned int j = 0; j < size; j++)
	//		tmpRedBlackTree.remove(nodes[j]);
	//	finish = high_resolution_clock::now();
	//	elapsed_seconds = finish - start;
	//	cout << "Remove BTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;*/

	//	cout << endl;
	//}
	cout << "Generic tests ended." << endl << endl;
	return 0;
}
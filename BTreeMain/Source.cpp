// Using BTree implementation from here: http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
// Template class separation workarounds described here: https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp
// For time computation we're using chrono library: http://en.cppreference.com/w/cpp/chrono

#include <iostream>
#include <string>
#include <chrono>
#include <Windows.h>

#include "BTree.h"
#include "BTree.cpp" // To fix template class separation

#include "BTreeNode.h"
#include "BTreeNode.cpp" // To fix template class separation

#include "Dummy.h" // For comparison test!

#include "RedBlackTreeNode.h"

#include "RedBlackTree.h"
#include "RedBlackTree.cpp" // To fix template class separation

using namespace std;
using namespace std::chrono;

// (C++11 Style) Chrono start and end time points.
time_point<high_resolution_clock> start, finish;

int main()
{
	/*
		Testing how Chrono works...
	*/
	cout << "Testing chrono clock:" << endl;
	start = high_resolution_clock::now();
	Sleep(200);
	Sleep(30);
	Sleep(4);
	finish = high_resolution_clock::now();
	duration<double> elapsed_seconds = finish - start;
	cout << "Milliseconds format = " << duration_cast<milliseconds>(finish - start).count() << endl;
	cout << "Plain format = " << elapsed_seconds.count() << endl;
	cout << "End of chrono test." << endl << endl;

	/*
		Testing t parameter for BTree
	*/
	cout << "Testing T parameter for BTree:" << endl;
	for (unsigned int t = 10; t <= 1000; t *= 10)
	{
		// Fill it with random integers
		int* integers = new int[1000000];
		for (unsigned int j = 0; j < 1000000; j++)
			// Ranges from -1000 to 1000
			integers[j] = -1000 + (rand() % (int)(1000 - (-1000) + 1));

		BTree<int> tmpBTree(t);
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < 1000000; j++)
			tmpBTree.insert(integers[j]);
		finish = high_resolution_clock::now();
		duration<double> elapsed_seconds = finish - start;
		cout << "T param BTree insert " << t << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < 1000000; j++)
			tmpBTree.remove(integers[j]);
		finish = high_resolution_clock::now();
		elapsed_seconds = finish - start;
		cout << "T param BTree remove " << t << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;
		cout << endl;
	}

	/*
		Running simple tests for BTree and RedBlackTree (int)
	*/
	cout << "Testing insertions and deletions (for BTree and RedBlackTree):" << endl;
	BTree<int> tMillions(3), tTenMillions(3), tHundredMillions(3);
	for (unsigned int i = 1; i <= 100; i *= 10)
	{
		int size = i * 1000000;

		// Fill it with random integers
		int* integers = new int[size];
		RedBlackTreeNode<int>** nodes = new RedBlackTreeNode<int>*[size];
		for (unsigned int j = 0; j < size; j++)
		{
			// Ranges from -1000 to 1000
			integers[j] = -1000 + (rand() % (int)(1000 - (-1000) + 1));
			nodes[j] = new RedBlackTreeNode<int>(integers[j]);
		}

		// BTree (Insert)
		BTree<int> tmpBTree(3);
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpBTree.insert(integers[j]);
		finish = high_resolution_clock::now();
		duration<double> elapsed_seconds = finish - start;
		cout << "Insert BTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// RedBlackTree (Insert)
		RedBlackTree<int> tmpRedBlackTree;
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
		{
			tmpRedBlackTree.insert(nodes[j]);
		}
		finish = high_resolution_clock::now();
		elapsed_seconds = finish - start;
		cout << "Insert RedBlackTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// BTree (Remove)
		/*start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpBTree.remove(integers[j]);
		finish = high_resolution_clock::now();
		elapsed_seconds = finish - start;
		cout << "Remove BTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;*/

		// RedBlackTree (Remove)
		/*start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpRedBlackTree.remove(nodes[j]);
		finish = high_resolution_clock::now();
		elapsed_seconds = finish - start;
		cout << "Remove BTree " << i << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;*/

		cout << endl;
	}
	cout << "Generic tests ended." << endl << endl;

	/*
		Testing Dummy object and custom extractor + (optional) comparator
	*/
	cout << "Testing Dummy objest:" << endl;
	BTree<Dummy, DummyExtractor> t(3);
	//BTree<Dummy, DummyExtractor, DummyComparator> t(3);
	//BTree<Dummy, DummyExtractor, Less<Dummy, DummyExtractor>> t(3);

	Dummy** dummies = new Dummy*[22]{ new Dummy("D1", 1), new Dummy("D3", 3), new Dummy("D7", 7), new Dummy("D10", 10), new Dummy("D11", 11),
		new Dummy("D13", 13), new Dummy("D14", 14), new Dummy("D15", 15), new Dummy("D16", 16),	new Dummy("D19", 19), new Dummy("D24", 24),
		new Dummy("D25", 25), new Dummy("D26", 26),	new Dummy("D21", 21), new Dummy("D4", 4), new Dummy("D5", 5), new Dummy("D20", 20),
		new Dummy("D22", 22), new Dummy("D2", 2), new Dummy("D17", 17),	new Dummy("D12", 12), new Dummy("D6", 6) };

	for (unsigned int i = 0; i < 22; i++)
	{
		t.insert(*dummies[i]);
	}

	cout << "Traversal of tree constructed is\n";
	t.traverse();
	cout << endl;

	t.remove(*dummies[2]);
	cout << "Traversal of tree after removing D7 element\n";
	t.traverse();
	cout << endl;

	t.remove(*dummies[5]);
	cout << "Traversal of tree after removing D13 element\n";
	t.traverse();
	cout << endl;

	t.remove(*dummies[0]);
	cout << "Traversal of tree after removing D1 element\n";
	t.traverse();
	cout << endl;

	cout << "End of Dummy test." << endl << endl;
	return 0;
}
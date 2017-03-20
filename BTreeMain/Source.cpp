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
	start = high_resolution_clock::now();
	Sleep(200);
	Sleep(30);
	Sleep(4);
	finish = high_resolution_clock::now();
	duration<double> elapsed_seconds = finish - start;
	cout << "Milliseconds format = " << duration_cast<milliseconds>(finish - start).count() << endl;
	cout << "Plain format = " << elapsed_seconds.count() << endl;

	/*
		Running simple test on primitive type (int)
	*/
	BTree<int> tr1(3);
	int* integers = new int[1000000];
	for (unsigned int i = 0; i < 1000000; i++) 
	{
		integers[i] = i + 1;
		tr1.insert(integers[i]);
	}
	tr1.traverse();



	/*
		Testing Dummy object and custom extractor + (optional) comparator
	*/
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

	// Testing red-black tree implementation
	cout << "Red black tree test:" << endl;

	return 0;
}
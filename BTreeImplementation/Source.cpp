// Using BTree implementation from here: http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
// Template class separation workarounds described here: https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp

#include <iostream>
#include <string>

#include "BTree.h"
#include "BTree.cpp" // Necessary for template class separation

#include "BTreeNode.h"
#include "BTreeNode.cpp" // Necessary for template class separation

#include "Dummy.h" // For comparison test!

#include "RedBlackTreeNode.h"
#include "RedBlackTree.h"
#include "RedBlackTree.cpp" // Necessary for template class separation

using namespace std;

int main()
{
	BTree<Dummy, DummyComparator> t(3); // A B-Tree with minium degree 3

	Dummy** dummies = new Dummy*[22]{ new Dummy("D1", 1), new Dummy("D3", 3), new Dummy("D7", 7), new Dummy("D10", 10), new Dummy("D11", 11),
		new Dummy("D13", 13), new Dummy("D14", 14), new Dummy("D15", 15), new Dummy("D16", 16),	new Dummy("D19", 19), new Dummy("D24", 24),
		new Dummy("D25", 25), new Dummy("D26", 26),	new Dummy("D21", 21), new Dummy("D4", 4), new Dummy("D5", 5), new Dummy("D20", 20),
		new Dummy("D22", 22), new Dummy("D2", 2), new Dummy("D17", 17),	new Dummy("D12", 12), new Dummy("D6", 6) };

	for (unsigned int i = 0; i < 22; i++)
	{
		t.insert(dummies[i]);
	}

	cout << "Traversal of tree constructed is\n";
	t.traverse(Dummy::printableValueFunc);
	cout << endl;

	t.remove(dummies[2]);
	cout << "Traversal of tree after removing D7 element\n";
	t.traverse(Dummy::printableValueFunc);
	cout << endl;

	t.remove(dummies[5]);
	cout << "Traversal of tree after removing D13 element\n";
	t.traverse(Dummy::printableValueFunc);
	cout << endl;

	t.remove(dummies[0]);
	cout << "Traversal of tree after removing D1 element\n";
	t.traverse(Dummy::printableValueFunc);
	cout << endl;

	// Testing red-black tree implementation
	cout << "Red black tree test:" << endl;

	RedBlackTree<int> rbTree;

	RedBlackTreeNode<int>* n;
	for (int i = 0; i < 23; i++)
	{
		// n = new Node<int>(rand());
		n = new RedBlackTreeNode<int>(i + 1);
		rbTree.insert(n);
	}
	cout << rbTree.getSize() << endl;
	//pretty_print_tree(rbTree.getRoot(), rbTree.getNil());
	return 0;
}
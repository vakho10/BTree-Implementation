// Using BTree implementation from here: http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
// Template class separation workarounds described here: https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp

#include <iostream>

#include "BTree.h"
#include "BTreeNode.h"

#include "BTree.cpp" // Necessary for template class separation
#include "BTreeNode.cpp" // Necessary for template class separation

using namespace std;

int main() 
{
	BTree<int> t(3); // A B-Tree with minium degree 3

	t.insert(1);
	t.insert(3);
	t.insert(7);
	t.insert(10);
	t.insert(11);
	t.insert(13);
	t.insert(14);
	t.insert(15);
	t.insert(18);
	t.insert(16);
	t.insert(19);
	t.insert(24);
	t.insert(25);
	t.insert(26);
	t.insert(21);
	t.insert(4);
	t.insert(5);
	t.insert(20);
	t.insert(22);
	t.insert(2);
	t.insert(17);
	t.insert(12);
	t.insert(6);

	cout << "Traversal of tree constructed is\n";
	t.traverse();
	cout << endl;

	t.remove(6);
	cout << "Traversal of tree after removing 6\n";
	t.traverse();
	cout << endl;

	t.remove(13);
	cout << "Traversal of tree after removing 13\n";
	t.traverse();
	cout << endl;

	t.remove(7);
	cout << "Traversal of tree after removing 7\n";
	t.traverse();
	cout << endl;

	t.remove(4);
	cout << "Traversal of tree after removing 4\n";
	t.traverse();
	cout << endl;

	t.remove(2);
	cout << "Traversal of tree after removing 2\n";
	t.traverse();
	cout << endl;

	t.remove(16);
	cout << "Traversal of tree after removing 16\n";
	t.traverse();
	cout << endl;

	return 0;
}
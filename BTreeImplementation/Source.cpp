#include <iostream>

#include "BTree.h"

using namespace std;

/*
	Code (idea) source taken from here: http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
*/

int main()
{
	BTree<int> t(3); // A B-Tree with minium degree 3
	t.insert(10);
	t.insert(20);
	t.insert(5);
	t.insert(6);
	t.insert(12);
	t.insert(30);
	t.insert(7);
	t.insert(17);

	cout << "Traversal of the constucted tree is ";
	t.traverse();

	int k = 6;
	(t.search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";

	k = 15;
	(t.search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";

	return 0;
}
#include "../TestSuite/Utils.h"

#include <random>
#include <algorithm>
#include <functional>

#include <iostream>
#include "Helpers.h"

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
	for (int i = 1; i <= 10; i++)
	{
		t1->insert(i);
		t2->insert(i);
	}	

	// Test if both versions get the same results when searching keys
	for (int i = 1; i <= 10; i++)
	{
		int index1 = t1->search(i)->findKey(i);
		int index2 = t2->search(i)->findKey(i);
		cout << "ind1: " << index1 << ", ind2: " << index2 << endl;
	}
}
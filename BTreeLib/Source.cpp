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
	for (int i = 2; i <= 20; i+=2)
	{
		t1->insert(i);
		t2->insert(i);
	}	

	t2->insert(-2);
	t2->insert(3);

	//int k = 10;
	//int* keys = new int[3] { 12, 15, 16 };
	//int md, i = 0, st = 0, fin = 3;
	//i = fin;
	//while (st <= fin)
	//{
	//	md = (st + fin) / 2;
	//	if (k == keys[md])
	//	{
	//		fin = md - 1; // Goto left side and find another match
	//		i = fin;
	//	}
	//	else 
	//		if (k < keys[md]) // keys[middle] < k
	//		{
	//			fin = md - 1;
	//			i = fin;
	//		}
	//		else {
	//			st = md + 1;
	//		}
	//}
	//cout << i << endl;
}
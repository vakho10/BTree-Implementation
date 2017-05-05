#include "stdafx.h"
#include "CppUnitTest.h"

#include "../BTreeLib/OldBTree.h"

#include "../BTreeLib/BTree.h"
#include "../BTreeLib/BTree.cpp" // To fix template class separation

#include "../BTreeLib/BTreeNode.h"
#include "../BTreeLib/BTreeNode.cpp" // To fix template class separation

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace OldTree;
using namespace BTreeLib;

namespace TestSuite
{
	// Easy tests to determine the similarity between the old and the new versions of BTree
	TEST_CLASS(BTreeTests)
	{
	public:

		OldBTree* t1 = new OldBTree(3);
		BTree<int>* t2 = new BTree<int>(3);

		// Fill trees with test data before tests
		BTreeTests()
		{
			for (int i = 1; i <= 10; i++)
			{
				t1->insert(i);
				t2->insert(i);
			}
		}

		// Test if both versions get the same results when searching keys
		TEST_METHOD(TestSearch)
		{
			for (int i = 1; i <= 10; i++)
			{				
				int index1 = t1->search(i)->findKey(i);
				int index2 = t2->search(i)->findKey(i);
				Assert::AreEqual(index1, index2);
			}
		}

		// Test if both versions remove data correctly
		TEST_METHOD(TestRemove)
		{
			for (int i = 2; i <= 9; i++)
			{
				t1->remove(i);
				t2->remove(i);
			}
			// Finding 1
			Assert::AreEqual(t1->search(1)->findKey(1), 0);
			Assert::AreEqual(t2->search(1)->findKey(1), 0);

			// Finding 10
			Assert::AreEqual(t1->search(10)->findKey(10), 1);
			Assert::AreEqual(t2->search(10)->findKey(10), 1);
		}
	};
}
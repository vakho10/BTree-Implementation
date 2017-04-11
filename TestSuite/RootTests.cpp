#include "stdafx.h"
#include "CppUnitTest.h"

#include "OldBTree.h"

#include "BTree.h"
#include "BTree.cpp" // To fix template class separation

#include "BTreeNode.h"
#include "BTreeNode.cpp" // To fix template class separation

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestSuite
{
	// Tests which determine that everything works as before
	TEST_CLASS(RootTests)
	{
	public:

		OldTree::BTree* t1 = new OldTree::BTree(3);
		BTreeLib::BTree<int>* t2 = new BTreeLib::BTree<int>(3);

		RootTests()
		{
			for (int i = 1; i <= 10; i++)
			{
				t1->insert(i);
				t2->insert(i);
			}
		}

		TEST_METHOD(TestSearch)
		{
			for (int i = 1; i <= 10; i++)
			{
				int index1 = t1->search(i)->findKey(i);
				int index2 = t2->search(i)->findKey(i);
				std::cout << index1 << ", " << index2 << endl;
				Assert::AreEqual(index1, index2);
			}
		}

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
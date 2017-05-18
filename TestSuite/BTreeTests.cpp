#include "stdafx.h"
#include "CppUnitTest.h"

#include "../BTreeLib/OldBTree.h"

#include "../BTreeLib/BTree.h"
#include "../BTreeLib/BTree.cpp" // To fix template class separation

#include "../BTreeLib/BTreeNode.h"
#include "../BTreeLib/BTreeNode.cpp" // To fix template class separation

#include <algorithm>    // std::random_shuffle

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

		BTreeTests()
		{
			// Shuffle array with different elements and test insertion
			int *numbers = new int[100];
			for (int i = 1; i <= 100; i++)
			{
				numbers[i - 1] = i;
			}
			std::random_shuffle(numbers, numbers + 100);

			for (int i = 0; i < 100; i++)
			{
				t1->insert(numbers[i]);
				t2->insert(numbers[i]);				
			}
		}

		TEST_METHOD(TestSearch)
		{
			for (int i = 1; i <= 100; i++)
			{				
				int index1 = t1->search(i)->findKey(i);

				auto node = t2->search(i);
				
				int index2 = node->findKey(i) - node->getPositionOfFirstKey(); // reseting to zero position
				Assert::AreEqual(index1, index2);
			}
		}

		TEST_METHOD(TestRemove)
		{
			Assert::Fail(L"Test not implemented yet!");			
		}
	};
}
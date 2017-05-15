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

		BTreeTests()
		{
			for (int i = 2; i <= 30; i+=2)
			{
				t1->insert(i);
				t2->insert(i);				
			}

			for (int i = 1; i <= 30; i+=2)
			{
				t1->insert(i);
				t2->insert(i);
			}
		}

		TEST_METHOD(TestSearch)
		{
			for (int i = 1; i <= 30; i++)
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
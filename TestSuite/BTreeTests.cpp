#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

#include <algorithm>    // std::random_shuffle

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
	// ძველი და ახალი ხეების შედარება (ახალი იგივე შედეგს უნდა იძლეოდეს)
	TEST_CLASS(BTreeTests)
	{
	public:

		const int numberOfElements = 1000;
		int *numbers = new int[numberOfElements];

		OldBTree* t1 = new OldBTree(3);
		BTree<int>* t2 = new BTree<int>(3);

		BTreeTests()
		{
			for (int i = 0; i < numberOfElements; i++)
			{
				numbers[i] = i - (numberOfElements / 2);
			}
			std::random_shuffle(numbers, numbers + numberOfElements); // აურიე რიცხვები უკეთესი ტესტირებისთვის

			for (int i = 0; i < numberOfElements; i++)
			{
				t1->insert(numbers[i]);
				t2->insert(numbers[i]);
			}
		}

		TEST_METHOD(TestInsert) 
		{
			Assert::AreEqual(t1->traverse(), t2->traverse());
		}

		TEST_METHOD(TestSearch)
		{
			std::random_shuffle(numbers, numbers + numberOfElements);
			for (int i = 0; i < numberOfElements; i++)
			{
				auto node = t2->search(numbers[i]);

				int index1 = (t1->search(numbers[i])->findKey(numbers[i]) + node->getPositionOfFirstKey()) % node->getCapacity();
				int index2 = node->findKey(numbers[i]) % node->getCapacity();

				Assert::AreEqual(index1, index2);
			}

			Assert::AreEqual(t1->traverse(), t2->traverse()); // შეგვიძლია გადავლით შემოწმებაც!
		}

		TEST_METHOD(TestRemove)
		{
			std::random_shuffle(numbers, numbers + numberOfElements);

			for (int i = 0; i < numberOfElements; i++)
			{
				t1->remove(numbers[i]);
				t2->remove(numbers[i]);

				Assert::AreEqual(t1->traverse(), t2->traverse());
			}
		}
	};
}
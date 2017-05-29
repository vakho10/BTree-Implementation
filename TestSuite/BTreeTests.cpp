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

		const int numberOfElements = 10000;

		OldBTree* t1 = new OldBTree(3);
		BTree<int>* t2 = new BTree<int>(3);

		BTreeTests()
		{			
			int *numbers = new int[numberOfElements];
			for (int i = 1; i <= numberOfElements; i++)
			{
				numbers[i - 1] = i; // ვიღებთ განსხვავებულ რიცხვებს!
			}
			std::random_shuffle(numbers, numbers + numberOfElements); // აურიე რიცხვები უკეთესი ტესტირებისთვის
			 
			for (int i = 0; i < numberOfElements; i++)
			{
				t1->insert(numbers[i]);
				t2->insert(numbers[i]);				
			}
		}

		TEST_METHOD(TestSearch)
		{
			for (int i = 1; i <= numberOfElements; i++)
			{				
				int index1 = t1->search(i)->findKey(i);								
				
				auto node = t2->search(i);				
				int index2 = node->findKey(i) - node->getPositionOfFirstKey(); // ინდექსირების გასწორება
				
				Assert::AreEqual(index1, index2);
			}
			Assert::AreEqual(t1->traverse(), t2->traverse()); // შეგვიძლია გადავლის შედეგით შემოწმებაც!
		}

		TEST_METHOD(TestRemove)
		{
			// The traverse function has the same output for the both of these B-trees!
			for (int i = 1; i <= numberOfElements; i++)
			{
				t1->remove(i);
				t2->remove(i);

				Assert::AreEqual(t1->traverse(), t2->traverse());
			}
		}
	};
}
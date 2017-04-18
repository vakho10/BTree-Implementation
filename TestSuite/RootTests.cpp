#include "stdafx.h"
#include "CppUnitTest.h"

#include <random>
#include <algorithm>

#include "../BTreeLib/OldBTree.h"

#include "../BTreeLib/BTree.h"
#include "../BTreeLib/BTree.cpp" // To fix template class separation

#include "../BTreeLib/BTreeNode.h"
#include "../BTreeLib/BTreeNode.cpp" // To fix template class separation

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace OldTree;
using namespace BTreeLib;

namespace TestSuite
{
	// Utility functions
	class Utils
	{
	public:
		static int findKeyUsingWhile(int k, int* arr, int size)
		{
			int i = 0;
			while (i < size && arr[i] < k)
				++i;
			return i;
		}

		static int findKeyUsingBinary(int k, int* arr, int size)
		{
			int left = 0, right = size - 1, middle, i = 0;
			while (left <= right)
			{
				middle = (left + right) / 2;
				if (k == arr[middle])
				{
					i = middle;
					right = middle - 1; // Goto left side and find another match
				}
				else
					if (k < arr[middle])
						right = middle - 1;
					else {
						left = middle + 1;
						i = left;
					}
			}
			return i;
		}
	};

	// Tests which determine that everything works as before
	TEST_CLASS(RootTests)
	{
	public:

		OldBTree* t1 = new OldBTree(3);
		BTree<int>* t2 = new BTree<int>(3);

		RootTests()
		{
			for (int i = 1; i <= 10; i++)
			{
				t1->insert(i);
				t2->insert(i);
			}
		}

		TEST_METHOD(TestBinarySearch)
		{
			int size = 1000, whileIndex, binaryIndex;
			int* arr = new int[size];

			// Generate random numbers
			random_device rd;
			mt19937 rng(rd());
			uniform_int_distribution<int> uni(1, 50); // [1, 50]
			for (size_t i = 0; i < size; i++) {
				arr[i] = uni(rng);
			}
			sort(arr, arr + size);

			// Existing
			uniform_int_distribution<int> uni2(0, size - 1);
			for (size_t i = 0; i < size; i++) 
			{
				int index = uni2(rng);
				whileIndex = Utils::findKeyUsingWhile(arr[index], arr, size);
				binaryIndex = Utils::findKeyUsingBinary(arr[index], arr, size);
				Assert::AreEqual(whileIndex, binaryIndex);
			}

			// Nonexisting (first)
			whileIndex = Utils::findKeyUsingWhile(1000, arr, size);
			binaryIndex = Utils::findKeyUsingBinary(1000, arr, size);
			Assert::AreEqual(whileIndex, binaryIndex);

			// Nonexisting (last)
			whileIndex = Utils::findKeyUsingWhile(-1000, arr, size);
			binaryIndex = Utils::findKeyUsingBinary(-1000, arr, size);
			Assert::AreEqual(whileIndex, binaryIndex);
		}

		TEST_METHOD(TestSearch)
		{
			for (int i = 1; i <= 10; i++)
			{
				int index1 = t1->search(i)->findKey(i);
				int index2 = t2->search(i)->findKey(i);
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
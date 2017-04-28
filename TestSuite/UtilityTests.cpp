#include "stdafx.h"
#include "CppUnitTest.h"

#include "Utils.h"

#include <random>
#include <algorithm>

#include <iostream>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestSuite
{
	TEST_CLASS(UtilityTests)
	{
	public:

		// Test if the new (changed) binary search code works correctly 
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

		// Test new, circular array representation
		TEST_METHOD(TestCircularArray)
		{
			int capacity = 4;
			CircularArray<int>* newArr = new CircularArray<int>(capacity);

			for (size_t i = 1; i <= capacity; i++)
				newArr->insert(i);

			// Test insertion
			for (int i = 0; i < capacity; i++)
				Assert::AreEqual(i + 1, newArr->keys[i]); // [st_1, 2, 3, fn_4]

			// Test positionOfFirstKey movements
			newArr->erase(1); // [1, st_2, 3, fn_4]
			newArr->erase(2); // [1, 2, st_3, fn_4]
			Assert::AreEqual(2, newArr->positionOfFirstKey);

			newArr->insert(-1); // [1, st_-1, 3, fn_4]
			Assert::AreEqual(1, newArr->positionOfFirstKey); 

			newArr->erase(4); // [1, st_-1, fn_3, 4]
			newArr->insert(2); // [1, st_-1, 2, fn_3]
			Assert::AreEqual(2, newArr->keys[newArr->positionOfFirstKey + 1]);

			newArr->insert(1); // [fn_3, st_ - 1, 1, 2]
			Assert::AreEqual(1, newArr->keys[newArr->positionOfFirstKey + 1]);

			newArr->erase(4); // 4 doesn't exist, nothing changed -> [fn_3, st_-1, 1, 2]
			newArr->erase(2); // [3, st_ - 1, 1, fn_3]
			newArr->erase(1); // [3, -1, st_-1, fn_3]
			newArr->insert(-16); // [3, st_-16, -1, fn_3]
			Assert::AreEqual(-16, newArr->keys[newArr->positionOfFirstKey]);

			newArr->insert(-14); // [fn_3, st_-16, -14, -1]
			Assert::AreEqual(-14, newArr->keys[newArr->positionOfFirstKey + 1]);
			Assert::AreEqual(-1, newArr->keys[newArr->positionOfFirstKey + 2]);

			newArr->erase(-14); // [fn_3, -16, st_-16, -1]
			newArr->erase(-16); // [fn_3, -16, -16, st_-1]
			newArr->insert(20); // [3, fn_20, -16, st_-1]
			newArr->insert(16); // [3, 16, fn_20, st_-1]

			Assert::AreEqual(3, newArr->keys[0]);
			Assert::AreEqual(16, newArr->keys[1]);
			Assert::AreEqual(20, newArr->keys[2]);
			Assert::AreEqual(-1, newArr->keys[3]);
		}
	};
}
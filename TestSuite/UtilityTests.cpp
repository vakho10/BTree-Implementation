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
			int size = 5, whileInd, binInd;
			int* arr1 = new int[size] {0, 1, 2, 2, 3}; // 1 -> index:2
			int* arr2 = new int[size] {0, 1, 2, 3, 3}; // 2 -> index:3
			int* arr3 = new int[size] {1, 1, 2, 3, 3}; // 0 -> index:0 (first)
			int* arr4 = new int[size] {1, 1, 2, 3, 4}; // 4 -> index:4 (last)

			// While test 
			Assert::AreEqual(2, Utils::findKeyUsingWhile(1, arr1, size));
			Assert::AreEqual(3, Utils::findKeyUsingWhile(2, arr2, size));
			Assert::AreEqual(0, Utils::findKeyUsingWhile(0, arr3, size));
			Assert::AreEqual(5, Utils::findKeyUsingWhile(4, arr4, size));

			// Test 1
			whileInd = Utils::findKeyUsingWhile(1, arr1, size);
			binInd = Utils::findKeyUsingBinary(1, arr1, size);
			Assert::AreEqual(whileInd, binInd);

			// Test 2
			whileInd = Utils::findKeyUsingWhile(2, arr2, size);
			binInd = Utils::findKeyUsingBinary(2, arr2, size);
			Assert::AreEqual(whileInd, binInd);

			// Test 3 (nonexisting first)
			whileInd = Utils::findKeyUsingWhile(0, arr3, size);
			binInd = Utils::findKeyUsingBinary(0, arr3, size);
			Assert::AreEqual(whileInd, binInd);

			// Test 4 (nonexisting last)
			whileInd = Utils::findKeyUsingWhile(4, arr4, size);
			binInd = Utils::findKeyUsingBinary(4, arr4, size);
			Assert::AreEqual(whileInd, binInd);

			// Random dynamic tests
			random_device rd;
			mt19937 rng(rd());
			uniform_int_distribution<int> uni(1, 50); // [1, 50]
			int* arr = new int[100];
			for (size_t i = 0; i < 100; i++) 
				arr[i] = uni(rng);
			
			sort(arr, arr + size);

			// Check for existing and nonexisting elements
			uniform_int_distribution<int> uni2(0, size + 2);
			for (size_t i = 0; i < size; i++)
			{
				int index = uni2(rng);
				whileInd = Utils::findKeyUsingWhile(arr[index], arr, size);
				binInd = Utils::findKeyUsingBinary(arr[index], arr, size);
				Assert::AreEqual(whileInd, binInd);
			}
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
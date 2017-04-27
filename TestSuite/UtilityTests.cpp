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
			int capacity = 128;
			CircularArray<int> newArr(capacity);
			
			for (size_t i = 1; i <= capacity; i++)
			{
				newArr.insert(i);
			}

			for (size_t i = 0; i < newArr.keysNumber; i++)
			{
				cerr << newArr.keys[i] << endl;
			}
			
		}
	};
}
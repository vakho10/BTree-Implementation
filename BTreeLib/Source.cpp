#include "../TestSuite/Utils.h"

#include <random>
#include <algorithm>
#include <functional>

#include <iostream>
#include "Helpers.h"

using namespace std;

int main()
{
	// Testing minus one idea [(i - 1) mod size]
	int size = 5, capacity = 4;
	int* k1 = new int[size];
	double* c1 = new double[size + 1];

	/*
	  st_1       2       fn_3       *
	/      \   /   \   /      \   /   \
 0.5        1.5     2.5        3.5     *
	*/
	int startPos = 0;
	for (int i = startPos, num = 1; i < capacity + startPos; i++, num++)
	{
		int keyInd = i % capacity;
		int leftChild = modForMin(keyInd - 1, size + 1);
		int rightChild = keyInd;
		k1[keyInd] = num;
		c1[leftChild] = num - .5;
		c1[rightChild] = num + .5;
	}

	for (int i = 0; i < size; i++)
		cout << k1[i] << ", ";
	cout << endl;

	for (int i = 0; i < size + 1; i++)
		cout << c1[i] << ", ";
	cout << endl;

}
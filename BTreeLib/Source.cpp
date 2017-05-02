#include "../TestSuite/Utils.h"

#include <random>
#include <algorithm>
#include <functional>

#include <iostream>
#include "Helpers.h"
#include "../TestSuite/Utils.h"

using namespace std;

int main()
{
	// Testing minus one idea [(i - 1) mod size]
	int k_size = 5, c_size= k_size +1, capacity = 4;
	int* keys = new int[k_size];
	double* children = new double[c_size];

	/*
	 3     fn_4     *     st_1     2
	/ \   /    \   / \   /    \   / \
 <-    3.5      4.5   0.5      1.5   2.5
	*/
	Utils::fillArrayForTests(3, capacity, k_size, c_size, keys, children);
	for (int i = 0; i < k_size; i++)
		cout << keys[i] << ", ";
	cout << endl;

	for (int i = 0; i < c_size; i++)
		cout << children[i] << ", ";
	cout << endl;

}
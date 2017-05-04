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
	int size = 4, capacity = 6;
	int* keys = new int[capacity] { 4, 6, -10, -10, 4, 4 };
	int st = 4;
	int fin = st + size - 1;
	// 3, fn_4, *, st_1, 2,
	
	
	for (int i = 0; i < capacity; i++)
		cout << keys[i] << ", ";
	cout << endl;
		
	int index = Utils::find_ind_inNode(keys, 4, st, fin, capacity);
	cout << "Index is " << index << endl;

}
#include "../TestSuite/Utils.h"

#include <random>
#include <algorithm>

#include <iostream>

using namespace std;

int main()
{
	int capacity = 4;
	CircularArray<int>* newArr = new CircularArray<int>(capacity);

	for (size_t i = 1; i <= capacity; i++)
		newArr->insert(i);
	newArr->print();

	newArr->erase(1); // [*, st_2, 3, fn_4]
	newArr->print();

	newArr->erase(2); // [*, *, st_3, fn_4]
	newArr->print();

	newArr->insert(-1);
	newArr->print();

	newArr->erase(4); // [*, st_-1, fn_3, *]
	newArr->print();

	newArr->insert(2); // [*, st_-1, 2, fn_3]
	newArr->print();

	newArr->insert(1);
	newArr->print();

	newArr->erase(4);
	newArr->print();

	newArr->erase(2);
	newArr->print();

	newArr->erase(1);
	newArr->print();

	newArr->insert(-16);
	newArr->print();

	newArr->insert(-14); // [st_-16, -15, fn_-1, *]
	newArr->print();

	newArr->erase(-14); // [*, st_-16, -15, fn_-1]
	newArr->print();

	newArr->erase(-16); // [*, *, st_-15, fn_-1]
	newArr->print();

	newArr->insert(20); // [fn_20, *, st_-15, -1]
	newArr->print();

	newArr->insert(16); // [16, fn_20, st_-15, fn_-1]
	newArr->print();
}
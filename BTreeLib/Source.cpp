#include "../TestSuite/Utils.h"

#include <random>
#include <algorithm>

#include <iostream>

using namespace std;

int main() 
{
	int capacity = 8;
	CircularArray<int>* newArr = new CircularArray<int>(capacity);

	for (size_t i = 1; i <= capacity; i+=2)
	{
		newArr->insert(i);
	}
	newArr->insert(-1);
	newArr->insert(2);
	newArr->insert(-3);
	newArr->insert(10);

	for (size_t i = 0; i < newArr->arrCapacity; i++)
	{
		cout << newArr->keys[i];
		if (i == newArr->positionOfFirstKey) {
			cout << " - st";
		}
		if (i == (newArr->positionOfFirstKey + newArr->keysNumber) % newArr->arrCapacity - 1) {
			cout << " - fn";
		}
		cout << endl;
	}
	cout << "--------" << endl;

	newArr->erase(1);
	newArr->erase(-3);

	for (size_t i = 0; i < newArr->arrCapacity; i++)
	{
		cout << newArr->keys[i];
		if (i == newArr->positionOfFirstKey) {
			cout << " - st";
		}
		if (i == (newArr->positionOfFirstKey + newArr->keysNumber) % newArr->arrCapacity - 1) {
			cout << " - fn";
		}
		cout << endl;
	}
}
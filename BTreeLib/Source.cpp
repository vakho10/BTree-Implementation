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

	newArr->erase(1);
	newArr->erase(2);

	newArr->insert(-1);

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

	
}
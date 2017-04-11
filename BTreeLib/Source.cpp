#include <iostream>

#include "BTree.h"
#include "BTree.cpp" // To fix template class separation

#include "BTreeNode.h"
#include "BTreeNode.cpp" // To fix template class separation

using namespace std;

template<class T>
T bin(T a[20], T n, T t)
{
	int left(0), right(n - 1), middle, i(0);
	while (left <= right)
	{
		middle = (left + right) / 2;
		if (t == a[middle]) 
		{
			i = middle;			
			right = middle - 1; // Goto left side and find another match
		}
		else
			if (t < a[middle])
				right = middle - 1;
			else
				left = middle + 1;
	}
	return i;
}

int main()
{
	/*int a[20], n, t, p, i;
	cout << "\nEnter number of elements :";
	cin >> n;
	cout << "\nAs binary search algorithm works in sorted array!";
	cout << "\nEnter " << n << " values in ascending order : ";
	for (i = 0;i < n;i++)
		cin >> a[i];
	cout << "\nEnter the element to find : ";
	cin >> t;
	p = bin(a, n, t);
	if (p != -1)
		cout << "\nThe element is found at " << p << " position";
	else
		cout << "\nElement Not Found!!";
	system("pause");*/

	BTreeLib::BTree<int>* t2 = new BTreeLib::BTree<int>(3);

	for (int i = 1; i <= 10; i++)
	{
		int index2 = t2->search(i)->findKey(i);
		cout << index2 << endl;
	}

	return 0;
}
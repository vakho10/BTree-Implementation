#pragma once

// Utility class that contains utility functions
class Utils
{
public:

	// Function which finds key in array using while loop
	static int findKeyUsingWhile(int k, int* arr, int size)
	{
		int i = 0;
		while (i < size && arr[i] <= k)
			++i;
		return i;
	}

	// Function which finds key in array using modified binary search algorithm
	static int findKeyUsingBinary(int k, int* arr, int size)
	{
		int left = 0, right = size - 1, middle, i = 0;
		while (left <= right)
		{
			middle = (left + right) / 2;
			if (k == arr[middle])
			{
				left = middle + 1;
				i = left;
			}
			else
				if (k < arr[middle]) {
					right = middle - 1;
					i = middle;
				}
				else {
					left = middle + 1;
					i = left;
				}
		}
		return i;
	}
};

template<typename T>
struct CircularArray
{
	T* keys = nullptr;			// Sorted array of elements
	int positionOfFirstKey;		// Index of the first key
	int keysNumber;				// Number of keys
	int arrCapacity;			// Maximum possible amount of keys in array (Must be EVEN and POWER OF 2)

	CircularArray(int _capacity) 
	{
		positionOfFirstKey = 0;
		keysNumber = 0;
		arrCapacity = _capacity;		
	}
	~CircularArray() 
	{ 
		free(keys); 
		keys = nullptr;
	}

	void insert(T k);
	void erase(T k);
	int findIndexInKeys(T key, int st, int fin);
	int searchInKeys(T k);
	void print();
};

template<typename T>
void CircularArray<T>::insert(T k)
{
	// No element situation
	if (keys == nullptr)
	{
		keys = (T*)malloc(sizeof(T)*(arrCapacity));
		keys[positionOfFirstKey] = k;
		keysNumber++;
		return;
	}

	int st = positionOfFirstKey;
	int fin = st + keysNumber - 1; // WAS: st + keysNumber - 1
	int ind = findIndexInKeys(k, st, fin);

	// ვარკვევთ თუ რომელ ბოლოსთან უფრო ახლოსაა ჩასამატებელი ადგილის ინდექსი
	if (ind - st >= fin - ind)
	{
		// სადაც უნდა ყოფილიყო, თუ უფრო პატარა გასაღები დახვდა, მაშინ  მარჯვნივ გაიწევს ინდექსი 
		if (k > keys[ind % arrCapacity])
			++ind;

		// ზოგიერთი გასაღების გაწევა, მარჯვენა ბოლოდან დაწყებული, რომ ადგილი გავანთავისუფლოთ
		while (ind <= fin)
		{
			keys[(fin + 1 + arrCapacity) % arrCapacity] = keys[(fin + arrCapacity) % arrCapacity];
			--fin;
		}
		keys[ind % arrCapacity] = k;
	}
	// თუ თავთან უფრო ახლოა, მაშინ ელემენტების ნაწილია მარცხნივ გადმოსაწევი
	else
	{
		// პირველი გასაღების პოზიცია მარცხნივ გაიწევს
		positionOfFirstKey = (st - 1 + arrCapacity) % arrCapacity;

		// გასაღები სადაც უნდა ყოფილიყო, თუ უფრო დიდი გასაღები დახვდა, მაშინ  მარცხნივ გაიწევს ინდექსი 
		if (k < keys[ind % arrCapacity]) 
			--ind;

		while (st <= ind)
		{
			keys[(st - 1 + arrCapacity) % arrCapacity] = keys[st % arrCapacity];
			++st;
		}
		keys[(ind + arrCapacity) % arrCapacity] = k;
	}
	++keysNumber;
}

template<typename T>
inline void CircularArray<T>::erase(T k)
{
	int st = positionOfFirstKey;
	int fin = st + keysNumber - 1;
	int ind = searchInKeys(k);

	if (-1 == ind) 
		return;

	if (ind - st > fin - ind)
	{
		while (fin != ind)
		{
			keys[ind % arrCapacity] = keys[(ind + 1) % arrCapacity];
			++ind;
		}
	}
	else
	{
		while (st != ind)
		{
			keys[ind % arrCapacity] = keys[(ind - 1) % arrCapacity];
			--ind;
		}
		positionOfFirstKey = (st + 1) % arrCapacity;
	}
	--keysNumber;
}

template<typename T>
int CircularArray<T>::findIndexInKeys(T k, int st, int fin)
{
	if (st == fin) 
		return st;

	int middle = (st + fin) / 2;

	if (keys[middle % arrCapacity] < k)
		return findIndexInKeys(k, middle + 1, fin);

	return findIndexInKeys(k, st, middle);
}

template<typename T>
inline int CircularArray<T>::searchInKeys(T k)
{
	int st = positionOfFirstKey;
	int fin = st + keysNumber - 1;
	int ind = findIndexInKeys(k, st, fin);
	if (k == keys[ind % arrCapacity]) 
		return ind;

	return -1;
}

template<typename T>
inline void CircularArray<T>::print()
{
	for (size_t i = 0; i < arrCapacity; i++)
	{
		if (i == 0) cout << "[";
		if (i == positionOfFirstKey) cout << "st_";
		else if (i == (positionOfFirstKey + keysNumber - 1) % arrCapacity) cout << "fn_";
		
		cout << keys[i];
		if (i != arrCapacity - 1) cout << ", ";
	}
	cout << "]" << endl;
}

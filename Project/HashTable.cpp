/*
Fazeli, Bijan
Lauguico, Marco

CS A200
March 9, 2017

Lab 6: HT Linear Probing (With ReHash and Delete)
*/

#include "HashTable.h"

// Overloading insertion operator 
ostream & operator << (ostream & out, const HashTable & hash)
{
	int temp;
	for (int i = 0; i < hash.capacity; ++i)
	{
		temp = hash.table[i];
		out << "Index " << i << ": ";
		switch (temp)
		{
		case -1:
			out << "E";
			break;
		case -2:
			out << "D";
			break;
		default:
			out << temp;
			break;
		}
		out << "\n";
	}

	return out;
}

HashTable::HashTable()
{
	capacity = CAPACITY;
	numOfElements = 0;
	table = new int[CAPACITY];

	for (int i = 0; i < CAPACITY; ++i)
		table[i] = -1;
}

HashTable::HashTable(int cap)
{
	capacity = cap;
	numOfElements = 0;
	table = new int[cap];

	for (int i = 0; i < cap; ++i)
		table[i] = -1;
}

HashTable::HashTable(const HashTable & otherHash)
{
	capacity = otherHash.capacity;
	numOfElements = otherHash.numOfElements;
	table = new int[otherHash.capacity];

	for (int i = 0; i < otherHash.capacity; ++i)
		table[i] = otherHash.table[i];
}

void HashTable::insert(int key)
{
	if (numOfElements >= static_cast<int>(capacity * 0.5)) //If numOfElem is >= 50% of capacity, do rehash.
		rehash();
	
	int index = hashValue(key);
	bool added = false;

	while (!added)
	{
		if (table[index] == -1 || table[index] == -2)
		{
			table[index] = key;
			++numOfElements;
			added = true;
		}
		else
			index = (index == (capacity - 1)) ? 0 : index + 1;
	}
}

bool HashTable::search(int key) const
{
	if (numOfElements != 0) {
		int index = hashValue(key), counter = 0;
		while (counter < capacity && table[index] != -1)
		{
			if (table[index] == key)
				return true;

			index = (index == (capacity - 1)) ? 0 : index + 1;
			++counter;
		}
	}

	return false;
}

void HashTable::deleteItem(int key)
{
	if (numOfElements != 0) {
		int index = hashValue(key), counter = 0;
		bool isDeleted = false;
		while (!isDeleted && counter < capacity && table[index] != -1)
		{
			if (table[index] == key) 
			{
				table[index] = -2;
				--numOfElements;
				isDeleted = true;
			}

			index = (index == (capacity - 1)) ? 0 : index + 1;
			++counter;
		}
		if (isDeleted)
			cout << key << " has been successfully deleted in the hash table!" << endl;
		else
			cout << "There is no existing " << key << " in the hash table." << endl;
	}
	else
	{
		cerr << "Error: Hash table is empty." << endl;
	}
}

int HashTable::getCapacity() const
{
	return capacity;
}

HashTable HashTable::operator=(const HashTable & otherHash)
{
	if (this != &otherHash)
	{
		if (capacity != otherHash.capacity)
		{
			delete[] table;
			capacity = otherHash.capacity;
			table = new int[otherHash.capacity];
		}

		for (int i = 0; i < otherHash.capacity; ++i)
			table[i] = otherHash.table[i];

		numOfElements = otherHash.numOfElements;
	}
	else
		cerr << "Attempted assignment to itself.";

	return *this;
}

int HashTable::operator[](int index) const
{
	return table[index];
}

HashTable::~HashTable()
{
	if (table != nullptr) 
	{
		delete[] table;
		table = NULL;
	}
}

int HashTable::hashValue(int key) const
{
	return key % capacity;
}

bool HashTable::isPrime(int number) const
{
	if ((number % 2 == 0) || (number % 3 == 0))
		return false;

	int i = 5, w = 2;

	while (i * i <= number)
	{
		if (number % i == 0)
			return false;

		i += w;
		w = 6 - w;
	}

	return true;
}

void HashTable::rehash()
{
	int newCap = capacity * 2;
	while (!isPrime(newCap))
		++newCap;

	HashTable oldTable(*this);
	delete[] table;
	capacity = newCap;
	numOfElements = 0;
	table = new int[newCap];

	for (int i = 0; i < newCap; ++i)
		table[i] = -1;

	if (oldTable.numOfElements != 0)
	{
		int index = 0;
		while (numOfElements != oldTable.numOfElements)
		{
			if (oldTable[index] != -1 && oldTable[index] != -2)
			{
				insert(oldTable[index]);
			}
			++index;
		}
	}
}

/**
  *
  * Assignment 2 for ECE430.217 Data Structures
  *
 **/
#include <iostream>

using namespace std;


#define INIT_TABLE_SIZE 64
#define MAX_LOAD 0.5

//
// HashTable data structure using linear/quad probing to handle collision
//
template <typename K, typename V>
class HashProbe
{
public:
	// Constructor
	HashProbe(unsigned int s = INIT_TABLE_SIZE) {
		table_size = s;
		num_elem = 0;
		ht = new TableElem<K, V>[table_size];
	};

	// Destructor
	~HashProbe() { removeall(); };

	unsigned int get_table_size() { return table_size; };
	unsigned int get_num_elem() { return num_elem; };
	bool isEmpty() { return (num_elem == 0); };
	double get_load_factor() { return (double)num_elem / table_size; };
	TableElem<K, V>* hash_table() { return ht; };


	void print() {
		for (int i = 0; i < table_size; i++) {
			if (!ht[i].empty) {
				std::cout << "*************************" << std::endl;
				std::cout << "Table index: " << i << std::endl;
				std::cout << ht[i].key << ":" << ht[i].val << std::endl;
			}
		}
	};


	// Find the element with the key
	TableElem<K, V>* find(const K& key);

	// Insert the element with the key and val
	// If the key already exists in the table, only replace the value with the val
	// Return the number of probing step
	int insert(const K& key, const V& val);

	// Remove the element with the key
	// Return true if it is successfully removed
	// return false if there is no elem with the key in the table
	bool remove(const K& key);

private:
	TableElem<K, V>* ht;
	unsigned int table_size, num_elem;

	// Hash function for string
	unsigned int hashfunction(const K& key) {
		unsigned int hash = 0;
		for (int i = 0; i < key.length(); ++i) hash = 12347 * hash + key[i];
		return hash % table_size;
	}

	const HashProbe& operator=(const HashProbe& other);

	virtual unsigned int get_pos(unsigned int pos, unsigned int step) = 0;

	// Enlarging the table when a load factor > MAX_LOAD
	void enlarge_table();

	// For destructor
	void removeall();
};

template <typename K, typename V>
class LinearHashProbe : public HashProbe<K, V> {
private:
	// Get a position from first position (hash) after step-th linear probing
	virtual unsigned int get_pos(unsigned int pos, unsigned int step) {
		unsigned int newval = pos + step;
		return newval;
	}
};

template <typename K, typename V>
class QuadHashProbe : public HashProbe<K, V> {
private:
	// Get a position from first position (hash) after step-th quad probing
	virtual unsigned int get_pos(unsigned int pos, unsigned int step) {
		unsigned int newval = (pos + (step + (step * step)) / 2);
		return newval;
	}
};


template <typename K, typename V>
TableElem<K, V>* HashProbe<K, V>::find(const K& key)
{
	//finds the index of the given key using the given hashfunction
	unsigned int hval = hashfunction(key);
	unsigned int orig = hashfunction(key);

	//create a separate value to control the loop function, this should not exceed the max table size
	unsigned int count = 1;

	//if the space if filled with something & if the key is not equal, keep looping to find the value
	while (((ht[hval].empty == false) && (ht[hval].key != key)) || (ht[hval].removed == true)) {
		hval = get_pos(orig, count) % table_size;
		count++;
	}

	if (ht[hval].key == key)
		return &ht[hval];

	return nullptr;
}

template <typename K, typename V>
int HashProbe<K, V>::insert(const K& key, const V& val)
{
	//find the index of the given key
	unsigned int hval = hashfunction(key);
	unsigned int orig = hashfunction(key);

	//keeps track of the number of steps
	unsigned int coll = 1;

	//keeps track of number of collisions
	unsigned int tracker = 0;

	//check if key already exists in the table
	if (find(key) == nullptr) {
		while (!ht[hval].empty) {
			hval = get_pos(orig, coll) % table_size;
			coll++;
			tracker++;
		}
		//increase num of elements in the table
		num_elem++;

		//insert new value
		ht[hval].key = key;
		ht[hval].val = val;
		ht[hval].empty = false;
		ht[hval].removed = false;

		//check load factor, if greater than 0.5, enlarge the table
		if (get_load_factor() > MAX_LOAD)
			enlarge_table();

		return tracker;
	}
	//if the key is the same, update the value and empty
	ht[hval].val = val;
	ht[hval].empty = false;

	return 0;
}

template <typename K, typename V>
bool HashProbe<K, V>::remove(const K& key)
{
	//check to see if it's an 'erased' key
	if (find(key)->removed == true) {
		return false;
	}

	//otherwise, go through the loop to find the key
	else {
		if (find(key) != nullptr) {
			num_elem--;
			
			find(key)->empty = true;
			find(key)->removed = true;
			return true;
		}
		else return false;
	}
}

template <typename K, typename V>
void HashProbe<K, V>::enlarge_table()
{
	//save the old table to a temporary table
	TableElem<K, V>* prevt = new TableElem<K,V>[table_size];

	//assign the old table size to a separate value
	unsigned int oldsize = table_size;

	//assign the original table to the temporary one
	for (unsigned int i = 0; i < oldsize; i++) {
		if (!ht[i].empty) {
			prevt[i].key = ht[i].key;
			prevt[i].val = ht[i].val;
			prevt[i].removed = ht[i].removed;
			prevt[i].empty = ht[i].empty;
		}
	}

	//delete the old table
	delete[] ht;
	//ht = nullptr;

	//set num of elements to 0
	num_elem = 0;

	//double the table size
	table_size = 2 * table_size;

	//update the original table using the new table_size
	ht = new TableElem<K, V>[table_size];


	//go through the old table to insert it into the new one
	for (unsigned int i = 0; i < oldsize; i++) {
		if (!prevt[i].empty)
			insert(prevt[i].key, prevt[i].val);
	}

	delete[] prevt;
	//prevt = nullptr;
}

template <typename K, typename V>
void HashProbe<K, V>::removeall()
{
	delete[] ht;
	//ht = nullptr;
}


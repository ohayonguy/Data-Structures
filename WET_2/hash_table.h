#ifndef __HASHTABLE__
#define __HASHTABLE__

#include <stdexcept>
#include <iostream>

class HashTableException : public std::exception {};
class KeyDoesNotExist : public HashTableException {};
class KeyAlreadyExists : public HashTableException {};

template<class Data>
class HashTable {
	/*
	*	represents a signe key-data pair stored inside.
	*	previous and next are pointers as part of chain hashing implementation.
	*/
	struct HashTableNode {
		int key;
		Data data;
		HashTableNode* previous, *next;

		/*
		*	constructor.
		*	@param key is the key for the new node.
		*	@param data is the data for the new node.
		*	@param previous is a pointer to the previous node, null if it is the first one.
		*	@param next is a pointer to the next node, null if it is the last one.
		*/
		HashTableNode(int key, const Data& data, HashTableNode* previous, HashTableNode* next) :
			key(key), data(data), previous(previous), next(next) {}
	};

	int size, num_of_keys;
	HashTableNode** table;

	/*
	*	the hash function.
	*	@param key is the key to hash.
	*	@returns the hashed value.
	*/
	int Hash(int key) {
		return key % size;
	}

	/*
	*	deletes all nodes with keys who are hashed to a given value.
	*	@param chain is the hash value by which keys will be deleted.
	*/
	void DeleteChain(int chain) {
		HashTableNode* chain_node = table[chain];
		while(chain_node != nullptr) {
			HashTableNode* temp = chain_node->next;
			delete chain_node;
			chain_node = temp;
			--num_of_keys;
		}
		table[chain] = nullptr;
	}

	/*
	*	looks for a key in a given chain.
	*	@param key is the key to search for.
	*	@param chain is the hash value of given key.
	*	@returns the data of the key if found.
	*/
	Data FindInChain(int key, int chain) {
		HashTableNode* current = table[chain];
		while(current != nullptr) {
			if (current->key == key) {
				return current->data;
			}
			current = current->next;
		}
		throw KeyDoesNotExist();
	}

	/*
	*	inserts a new key and data pair into a given chain.
	*	@param key is the new key to insert.
	*	@param data is the new data to insert.
	*	@param chain is the chain to insert to.
	*/
	void InsertInChain(int key, const Data& data, int chain) {
		if (table[chain] == nullptr) {
			table[chain] = new HashTableNode(key, data, nullptr, nullptr);
		}
		else if (table[chain]->key == key) {
			throw KeyAlreadyExists();
		}
		else {
			HashTableNode* current = table[chain];
			while (current->next != nullptr) {
				if ((current->next)->key == key) {
					throw KeyAlreadyExists();
				}
				current = current->next;
			}
			current->next = new HashTableNode(key, data, current, nullptr);
		}
	}

	/*
	*	deletes a key-data pair from a given chain.
	*	@param key is the key to delete.
	*	@param chain is the chain to delet efrom..
	*/
	void DeleteFromChain(int key, int chain) {
		HashTableNode* current_node = table[chain];
		while (current_node != nullptr) {
			if (current_node->key == key) {
				if (current_node->next) {
					(current_node->next)->previous = current_node->previous;
				}
				if (current_node->previous) {
					(current_node->previous)->next = current_node->next;
				}
				else { // deleting first node.
					table[chain] = current_node->next;
				}
				delete current_node;
				return;
			}
		}
		throw KeyDoesNotExist();
	}

	/*
	*	expands the table size by a constant factor
	*/
	void Expand() {
		ResizeTable(size * 2);
	}

	/*
	*	contracts the table size by a constant factor.
	*/
	void Contract() {
		ResizeTable(size / 2);
	}

	/*
	*	resizes the hash table, keeping all original key-data pairs.
	*	@param new_size is the new size of the table.
	*/
	void ResizeTable(int new_size) {
		HashTable<Data> new_table = HashTable<Data>(new_size);
		TransferTable(new_table);

		delete[] table;
		size = new_table.size;
		num_of_keys = 0;
		table = new HashTableNode*[size];
		for (int i = 0; i < size; ++i) {
			table[i] = nullptr;
		}

		new_table.TransferTable(*this);

		// no need to delete new table since it's a stack variable.
	}

	/*
	*	transfers an entire chain of key-data pairs into another hash table.
	*	@param chain is the chain number to transfer.
	*	@param destination_table is the table to transfer into.
	*/
	void TransferChain(int chain, HashTable& destination_table) {
		HashTableNode* current_node = table[chain];
		while(current_node != nullptr) {
			destination_table.Insert(current_node->key, current_node->data);
			current_node = current_node->next;
		}
		DeleteChain(chain);
	}

	/*
	*	transfers all the keys/data to another table.
	*	data is deleted from the original table.
	*	@param destination_table is the new table.
	*/
	void TransferTable(HashTable& destination_table) {
		if (this == &destination_table) {
			return;
		}

		for (int i = 0; i < size; ++i) {
			TransferChain(i, destination_table);
		}
	}

public:
	/*
	*	constructor.
	*	creates a new hash table with a given size.
	*	@param size is the size of the hash table.
	*/
	explicit HashTable(int size) : size(size), num_of_keys(0), table(new HashTableNode*[size]) {
		for (int i = 0; i < size; ++i) {
			table[i] = nullptr;
		}
	}

	/*
	*	destructor.
	*/
	~HashTable() {
		for (int i = 0; i < size; ++i) {
			DeleteChain(i);
		}
		delete[] table;
	}

	/*
	*	find the data of a given key in the table.
	*	@param key is the key to look for.
	*	@returns the data paired with the given key (if found).
	*/
	Data Find(int key) {
		int chain_index = Hash(key);
		return FindInChain(key, chain_index);
	}

	/*
	*	inserts a new key-data pair into the table (if it does not exist yet).
	*	@param key is the new key.
	*	@param data is the date for the new key.
	*/
	void Insert(int key, const Data& data) {
		int chain_index = Hash(key);
		InsertInChain(key, data, chain_index);

		++num_of_keys;
		if (num_of_keys == size) {
			Expand();
		}
	}

	/*
	*	deletes a given key and it's data from the table (if the key exists).
	*	@param key is the key to delete.
	*/
	void Delete(int key) {
		int chain_index = Hash(key);
		DeleteFromChain(key, chain_index);
		--num_of_keys;

		if (num_of_keys <= size/4) {
			Contract();
		}
	}

};

#endif //__HASHTABLE__
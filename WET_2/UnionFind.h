#ifndef __UNIONFIND__
#define __UNIONFIND__

#include <stdexcept>

class UnionFindException : public std::exception {};
class IllegalSet : public UnionFindException {};
class IllegalElement : public UnionFindException {};

template<class Data>
class UnionFind {
	typedef int setID;
	typedef int elementID;

	/*
	*	marks that a given element has no father,
	*	aka marks elements that are sets.
	*/
	const elementID NO_FATHER = -1;

	int size, *element_fathers, *set_sizes;
	Data *data_of_sets;

	/*
	*	returns the father of a given element.
	*	@param element is the wanted "son".
	*	@returns the ID of element's father.
	*/
	elementID GetFather(elementID element) {
		if (element < 0 || element >= size) {
			throw IllegalElement();
		}

		return element_fathers[element];
	}

	/*
	*	changes a given elements father.
	*	@param element is the "son".
	*	@param father is the new father value.
	*/
	void SetFather(elementID element, elementID father) {
		if (element < 0 || element >= size || father < 0 || father >= size) {
			throw IllegalElement();
		}

		element_fathers[element] = father;
	}

	/*
	*	finds the set to which a given element belongs.
	*	@param element is the ID to find set for.
	*	@returns the ID of the element's set.
	*/
	setID GetSet(elementID element) {
		while (GetFather(element) != NO_FATHER) {
			element = GetFather(element);
		}

		return (setID) element;
	}

	/*
	*	finds the size of a given set.
	*	@param set is the ID of the set.
	*	@returns the set size.
	*/
	int GetSize(setID set) {
		if (set < 0 || set >= size) {
			throw IllegalSet();
		}

		return set_sizes[set];
	}

	/*
	*	changes the size of a given set.
	*	@param set is the ID of the set.
	*	@param new_size is the new size value.
	*/
	void SetSize(setID set, int new_size) {
		if (set < 0 || set >= size) {
			throw IllegalSet();
		}

		set_sizes[set] = new_size;
	}

	/*
	*	performs a path shrinking from a given element all the way to it's set.
	*	aka makes all elements in the route to point at the final set.
	*	@param element is the element ID from which the shrinking begins.
	*/
	void ShrinkPath(elementID element) {
		setID set = GetSet(element);
		while (element != (elementID) set) {
			elementID father_temp = GetFather(element);
			SetFather(element, set);
			element = father_temp;
		}
	}

public:
	/*
	*	constructor.
	*	@param size is the number of elements in the structure.
	*/
	UnionFind(int size) : 	size(size), element_fathers(new int[size]), 
							set_sizes(new int[size]), data_of_sets(new Data[size]) {
								for (int i = 0; i < size; ++i) {
									element_fathers[i] = NO_FATHER;
									set_sizes[i] = 1;
								}
							}

	/*
	*	destructor
	*/
	~UnionFind() {
		delete[] element_fathers;
		delete[] set_sizes;
		delete[] data_of_sets;
	}

	/*
	*	finds the data that a given set represents.
	*	@param set is the ID of the set.
	*	@returns a copy of the set's data.
	*/
	Data GetData(setID set) {
		if (set < 0 || set >= size) {
			throw IllegalSet();
		}

		return data_of_sets[set];
	}

	/*
	*	changes the data that a given set represents.
	*	@param set is the ID of the set.
	*	@param data is the new data value.
	*/
	void SetData(setID set, Data data) {
		if (set < 0 || set >= size) {
			throw IllegalSet();
		}

		data_of_sets[set] = data;
	}

	/*
	*	finds the set to which a given element belongs.
	*	@param element is the ID of the element.
	* 	@returns the ID of the element's set.
	*/
	setID Find(elementID element) {
		setID result = GetSet(element);
		ShrinkPath(element);
		return result;
	}

	/*
	*	unites two given sets into one.
	*	@param set1 is the ID of the first set.
	*	@param set2 is the ID of the second set.
	*	@returns the ID of the union set.
	*/
	setID Union(setID set1, setID set2) {
		if (GetFather(set1) != NO_FATHER || GetFather(set2) != NO_FATHER ||
			set1 == set2) {
			throw IllegalSet();
		}

		if (GetSize(set1) < GetSize(set2)) {
			SetFather(set1, set2);
			SetSize(set2, GetSize(set2) + GetSize(set1));
			return set2;
		} else {
			SetFather(set2, set1);
			SetSize(set1, GetSize(set1) + GetSize(set2));
			return set1;
		}
	}

};





#endif // __UNIONFIND__
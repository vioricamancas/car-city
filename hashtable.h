/**
Copyright
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include <algorithm>
#include <vector>
#include <list>


#define list_type typename std::list<struct elem_info<Tkey, Tvalue> >

template <typename Tkey, typename Tvalue> struct elem_info {
	Tkey key;
	Tvalue value;
};

template <typename Tkey, typename Tvalue> class Hashtable {
 private:
	std::vector<std::list<struct elem_info<Tkey, Tvalue> > > H;
	// vector of buckets
	int HMAX;  // number of buckets
	unsigned int (*hash)(Tkey);  // pointer to hash function
	unsigned int size;  // number of elements in hashtable

 public:
	Hashtable(int hmax, unsigned int (*h)(Tkey)) {
		HMAX = hmax;
		this->hash = h;
		size = 0;
		H.resize(HMAX);
	}

	~Hashtable() {
	}

	void put(Tkey key, Tvalue value) {
		size++;
		int index = hash(key) % HMAX;
		elem_info<Tkey, Tvalue> aux;
		aux.key = key;
		aux.value = value;
		H[index].push_back(aux);
	}

	void remove(Tkey key) {
		size--;
		int index = hash(key) % HMAX;
		for (list_type::iterator it = H[index].begin(); it != H[index].end(); ++it)
			if (it->key == key){
				H[index].erase(it);
				return;
			}
	}

	Tvalue get(Tkey key) {
		int index = hash(key) % HMAX;
		for (list_type::iterator it = H[index].begin(); it != H[index].end(); ++it)
			if (it->key == key){
				return it->value;
			}
		return Tvalue();
	}

	bool has_key(Tkey key) {
		int index = hash(key) % HMAX;
		for (list_type::iterator it = H[index].begin(); it != H[index].end(); ++it)
			if (it->key == key){
				return true;
			}
		return false;
	}

	int get_size() {
		return size;
	}

	int get_capacity() {
		return HMAX;
	}

	int collisions(){
		if (size > HMAX)
			return size - HMAX;
		return 0;
	}
};

#endif  // HASHTABLE_H_

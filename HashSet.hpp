#pragma once

#include "Hash.hpp"
#include "StringRef.hpp"
#include <cmath>
#include <fstream>
#include <iostream>


// Hash capacity
static const int64_t HashSize = 524288; // Must be power of two

// Hash Index
struct HashSet {
	/// An entry inside the hash table
	struct Entry {
		int64_t hash;
		//int64_t length;
		//char worddata[24];
		Entry() : hash(0) {}
		void setDeleted() { hash=0; }
		bool isDeleted() const { return hash==0; }
	};

	/// Sizes
	int64_t size;
	int64_t capacity;
	int64_t mask;
	int64_t exp;
	/// The actual hash table
	Entry table[HashSize];

	/// Constructor
	HashSet(int64_t capacity) {
	    init(capacity);
	}

	HashSet() {}

	void init(int64_t capacity) {
	    size=0;
	    std::cout << capacity << std::endl;
	    std::cout << log(capacity) << std::endl;
	    std::cout << log(2) << std::endl;
	    exp = ceil(log(capacity)/log(2));
	    mask = pow(2,exp)-1;
	    this->capacity=mask+1;
	    //table.resize(this->capacity);
	    memset(table,0,sizeof(Entry)*HashSize);
	}

	  /// Destructor
	~HashSet() {
	    //std::cout << size << " " << capacity << std::endl;
	}

	/// Insert a kvp into the hash table
	bool insert(StringRef(word)) {
		auto hash=Crc32Hash()(word);
		auto slot=hash&mask;
		Entry* e = &table[slot];
		if (!e->isDeleted()) do {
		    slot = (slot + 1) & mask;
		    e=&table[slot];
		} while(!e->isDeleted());
		//memcpy(e->worddata,word.data(),word.length());
		//e->length=word.length();
		e->hash=hash;
		++size;
		return true;
	}

	/// Find an element by key
	bool find(StringRef word) const {
		auto hash=Crc32Hash()(word);
	    auto slot=hash&mask;
	    const Entry* e = &table[slot];
	    if (!e->isDeleted()) do {
	        if (e->hash == hash /* && StringRef(e->worddata,e->length) == word*/) return true;
	        slot = (slot + 1) & mask;
	        e=&table[slot];
	    } while(!e->isDeleted());
	    return false;
	}

	void write(const std::string& filename) {
		std::ofstream f(filename);
		f.write(reinterpret_cast<char*>(this),sizeof(HashSet));
	}
};

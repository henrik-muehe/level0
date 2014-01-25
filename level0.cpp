#include "BufferedReader.hpp"
#include "hash.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <cstring>

// Memory for the dictionary content
std::vector<char> dict;

// Hash capacity
static const int64_t HashSize = 524288; // Must be power of two

// Hash Index
struct HashSet {
	/// An entry inside the hash table
	struct Entry {
		int64_t hash;
		StringRef word;
		Entry() : hash(0),word(nullptr,0) {}
		void setDeleted() { word=StringRef(nullptr,0); }
		bool isDeleted() const { return word.length()==0; }
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
	    exp = ceil(log(capacity)/log(2));
	    mask = pow(2,exp)-1;
	    this->capacity=mask+1;
	    //table.resize(this->capacity);
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
		e->word=word;
		e->hash=hash;
		++size;
		return true;
	}

	/// Find an element by key
	bool find(StringRef word) {
		auto hash=Crc32Hash()(word);
	    auto slot=hash&mask;
	    Entry* e = &table[slot];
	    if (!e->isDeleted()) do {
	        if (e->hash == hash && e->word == word) return true;
	        slot = (slot + 1) & mask;
	        e=&table[slot];
	    } while(!e->isDeleted());
	    return false;
	}
};



int main(int argc,char* argv[]) {
	std::string path = (argc > 1) ? argv[1] : "/usr/share/dict/words";

	// Read dictionary into memory
	std::ifstream f(path);
	dict.insert(dict.end(),std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());

	// Read dictionary
	HashSet entries(300000);
	for (const char* start=dict.data(), *limit=dict.data()+dict.size(); start!=limit; ++start) {
		const char* wordStart=start;
		for (; start != limit && *start != '\n'; ++start);
		const char* wordEnd=start;
		entries.insert(StringRef(wordStart,std::distance(wordStart,wordEnd)));
	}

	// Read input and probe
	BufferedReader r(0);
	while(true) {
		char wordBuffer[5000];
		int l=r.readUntilSpace(wordBuffer);
		if (!l) break;
		std::string word(wordBuffer,l);
		std::string lword = word;
		std::transform(lword.begin(), lword.end(), lword.begin(), ::tolower);
		if (entries.find(StringRef(lword.c_str(),lword.length()))) {
			std::cout << word;
		} else {
			std::cout << "<" << word << ">";
		}
		l=r.readUntilNonSpace(wordBuffer);
		if (!l) break;
		std::cout << std::string(wordBuffer,l);
	}
	std::cout.flush();
}

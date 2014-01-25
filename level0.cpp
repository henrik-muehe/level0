#include "BufferedReader.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <cstring>

// Memory for the dictionary content
std::vector<char> dict;


// Hash Index
struct HashSet {
	/// An entry inside the hash table
	struct Entry {
		int64_t hash;
		const char* start;
		const char* end;
		Entry() : hash(0),start(nullptr),end(nullptr) {}
		void setDeleted() { start=nullptr; }
		bool isDeleted() const { return start==nullptr; }
	};

	/// Sizes
	int64_t size;
	int64_t capacity;
	int64_t mask;
	int64_t exp;
	/// The actual hash table
	std::vector<Entry> table;

	/// Constructor
	HashSet(int64_t capacity) {
	    init(capacity);
	}

	HashSet() {}

	void init(int64_t capacity) {
	    size=0;
	    exp = ceil(log(capacity)/log(2))+2;
	    mask = pow(2,exp)-1;
	    this->capacity=mask+1;
	    table.resize(this->capacity);
	}

	  /// Destructor
	~HashSet() {
	    //std::cout << size <<" "<< capacity << std::endl;
	}

	/// Insert a kvp into the hash table
	bool insert(const char* start, const char* end) {
		auto hash=std::hash<std::string>()(std::string(start,std::distance(start,end)));
		auto slot=hash&mask;
		Entry* e = &table[slot];
		if (!e->isDeleted()) do {
		    slot = (slot + 1) & mask;
		    e=&table[slot];
		} while(!e->isDeleted());
		e->start=start;
		e->end=end;
		e->hash=hash;
		++size;
		return true;
	}

	/// Find an element by key
	bool find(const char* start, const char* end) {
		auto len=std::distance(start,end);
		auto hash=std::hash<std::string>()(std::string(start,std::distance(start,end)));
	    auto slot=hash&mask;
	    Entry* e = &table[slot];
	    if (!e->isDeleted()) do {
	    	auto len2=std::distance(e->start,e->end);
	        if (e->hash == hash && len==len2 && memcmp(start,e->start,len) == 0) return true;
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
		entries.insert(wordStart,wordEnd);
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
		if (entries.find(lword.c_str(),lword.c_str()+lword.length())) {
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

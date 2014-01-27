#include "BufferedReader.hpp"
#include "HashSet.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <cstring>
#include <memory>

extern unsigned char _binary_bin_hashset_bin_start;
extern unsigned char _binary_bin_hashset_bin_end;
extern unsigned char _binary_bin_hashset_bin_size;


int main(int argc,char* argv[]) {
	HashSet& entries = *reinterpret_cast<HashSet*>(&_binary_bin_hashset_bin_start);

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

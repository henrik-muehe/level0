#include "BufferedReader.hpp"
#include "HashSet.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>


//// Memory for the dictionary content
static std::vector<char> dict;


int main(int argc,char* argv[]) {
	assert(argc==3);
	std::string path = (argc > 1) ? argv[1] : "/usr/share/dict/words";

	// Read dictionary into memory
	std::ifstream f(path);
	dict.insert(dict.end(),std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());

	// Read dictionary
	static HashSet entries(300000);
	for (const char* start=dict.data(), *limit=dict.data()+dict.size(); start!=limit; ++start) {
		const char* wordStart=start;
		for (; start != limit && *start != '\n'; ++start);
		const char* wordEnd=start;
		entries.insert(StringRef(wordStart,std::distance(wordStart,wordEnd)));
	}
	entries.write(argv[2]);
}

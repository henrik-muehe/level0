#include "BufferedReader.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>


std::vector<char> dict;





int main(int argc,char* argv[]) {
	std::string path = (argc > 1) ? argv[1] : "/usr/share/dict/words";

	// Read dictionary into memory
	std::ifstream f(path);
	dict.insert(dict.end(),std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());

	// Read dictionary
	std::unordered_set<std::string> entries;
	for (const char* start=dict.data(), *limit=dict.data()+dict.size(); start!=limit; ++start) {
		const char* wordStart=start;
		for (; start != limit && *start != '\n'; ++start);
		const char* wordEnd=start;
		entries.insert(std::string(wordStart, std::distance(wordStart,wordEnd)));
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
		if (entries.count(lword)) {
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

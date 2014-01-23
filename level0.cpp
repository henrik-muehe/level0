#include "BufferedReader.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>


int main(int argc,char* argv[]) {
	std::string path = (argc > 1) ? argv[1] : "/usr/share/dict/words";

	// Read dictionary
	std::unordered_set<std::string> entries;
	std::ifstream f(path);
	std::string lword;
	while (std::getline(f,lword)) {
		//std::transform(lword.begin(), lword.end(), lword.begin(), ::tolower);
		entries.insert(lword);
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





/*path = ARGV.length > 0 ? ARGV[0] : '/usr/share/dict/words'
entries = File.read(path).split("\n")

contents = $stdin.read
output = contents.gsub(/[^ \n]+/) do |word|
  if entries.include?(word.downcase)
    word
  else
    "<#{word}>"
  end
end
print output
*/
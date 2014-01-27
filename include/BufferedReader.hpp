#pragma once

#include <cstdint>
#include <unistd.h>


/// Buffered reader used to efficiently read simple structured input from stdin
class BufferedReader {
	/// Size of the input buffer
	static const uint64_t BufferSize = 1024*200;
	/// The input fd
	int input;
	/// The buffer
	char buffer[BufferSize];
	/// The current and end pointer
	const char* currentPtr;
	const char* endPtr;

public:
	/// Constructor
	BufferedReader(int input) : input(input),currentPtr(buffer),endPtr(buffer) {}
	/// Read more data into buffer
	bool replenish() {
		auto len=read(input,buffer,BufferSize);
		endPtr=buffer+len;
		currentPtr=buffer;
		return len>0;
	}
	/// Reads all bytes up to the next occurance of space into the buffer
	int64_t readUntilSpace(char* buffer) {
		auto* start=buffer;
		while (true) {
			for (; currentPtr!=endPtr; ++currentPtr) {
				if (*currentPtr==' '||*currentPtr=='\n') {
					goto done;
				}
				*buffer=*currentPtr;
				++buffer;
			}
			if (!replenish()) 
				break;
		}
		done:
		return buffer-start;
	}
	/// Reads all bytes up to the next occurance of non-space into the buffer
	int64_t readUntilNonSpace(char* buffer) {
		auto* start=buffer;
		while (true) {
			for (; currentPtr!=endPtr; ++currentPtr) {
				if (*currentPtr!=' '&&*currentPtr!='\n') {
					goto done;
				}
				*buffer=*currentPtr;
				++buffer;
			}
			if (!replenish()) 
				break;
		}
		done:
		return buffer-start;
	}
};

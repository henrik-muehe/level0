#include <x86intrin.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>


#define bool int
#define true (0==0)
#define false (0!=0)


// Hash a string of up to 24 characters
int computeHash(const char* from,size_t l) {

        if (l<=8) {
            return _mm_crc32_u64(0,((uint64_t*)from)[0]<<(64-8*l));
        } else if (l<=16) {
            return _mm_crc32_u64(_mm_crc32_u64(0,((uint64_t*)from)[0]),(((uint64_t*)from)[1])<<(128-8*l));
        } else {
			return _mm_crc32_u64(
						_mm_crc32_u64(
							_mm_crc32_u64(
								0,
								((uint64_t*)from)[0]
							),
							((uint64_t*)from)[1]
						),
						(((uint64_t*)from)[2])<<(128+64-8*l));
		}
		assert(false&&"Missing a case in hash.");
}


// Hash capacity
#define HashSize 524288

// Hash Set Entry
typedef struct {
	int32_t hash;
	//int64_t length;
	//char worddata[30];
} Entry;

// Is the entry marked deleted?
bool Entry_isDeleted(const Entry* e) {
	return e->hash == 0;
}

// Hash Set
typedef struct {
	/// Sizes
	int64_t size;
	int64_t capacity;
	int64_t mask;
	int64_t exp;
	/// The actual hash table
	Entry table[HashSize];
} HashSet;

// Find inside hash set
int HashSet_find(HashSet* h,const char* word,size_t len) {
	int32_t hash=computeHash(word,len);
	int64_t slot=hash&h->mask;
	const Entry* e = &h->table[slot];
	if (!Entry_isDeleted(e)) do {
		if (e->hash == hash) return true;
		slot = (slot + 1) & h->mask;
		e = &h->table[slot];
	} while (!Entry_isDeleted(e));
    return false;
}

extern char _binary_hashset_bin_start;
extern char _binary_hashset_bin_end;
extern char _binary_hashset_bin_size;

#define BufferSize (4096*8)

int main(int argc,char *argv[]) {
	HashSet* h = (HashSet*)(&_binary_hashset_bin_start);

	char buffer[BufferSize+1];
	char lowerWordBuffer[64];
	char wordBuffer[64];
	int readLen;
	int wordLen=0;
	int i;
	const char* start = 0, *end = 0;
	while((readLen = fread(buffer, 1, BufferSize, stdin)) > 0) {
  		for(i=0; i < readLen; i++) {
  			char c = buffer[i];
  			if (c != ' ' && c != '\n') {
  				if (c >= 'A' && c <= 'Z') lowerWordBuffer[wordLen] = buffer[i] - 'A' + 'a';
  				else lowerWordBuffer[wordLen] = buffer[i];
  				wordBuffer[wordLen++] = buffer[i];
  			} else {
  				if (wordLen > 0) {
  					wordBuffer[wordLen] = 0;
  					lowerWordBuffer[wordLen] = 0;
  					if (HashSet_find(h,lowerWordBuffer,wordLen)) {
	  					printf("%s", wordBuffer);
  					} else {
  						printf("<%s>", wordBuffer);
  					}
  					wordLen = 0;
  				}
  				printf("%c", c);
  			} 
  		}
  	}
}

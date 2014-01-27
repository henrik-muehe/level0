all: level0

bin/hashdumper: src/HashDumper.cpp $(wildcard include/*.hpp)
	mkdir -p bin
	g++ -I include -msse4.2  -std=c++11 -O3 -g -o $@ $<

bin/hashset.bin: bin/hashdumper data/dict.txt
	mkdir -p bin
	./bin/hashdumper data/dict.txt bin/hashset.bin

bin/hashset.o: bin/hashset.bin bin/hashdumper
	mkdir -p bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

level0: src/level0.c bin/hashdumper bin/hashset.o
	mkdir -p bin
	gcc -I include -msse4.2 -O3 -g -o $@ bin/hashset.o $<

#level0: src/level0.cpp bin/hashdumper bin/hashset.o
#	mkdir -p bin
#	g++ -I include -msse4.2 -O3 -g -o $@ bin/hashset.o $<

clean:
	rm -rf *.data* bin

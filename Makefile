all: level0

hashdumper: HashDumper.cpp *.hpp
	g++ -march=native -std=c++11 -O3 -g -o $@ $<

hashset.bin: hashdumper dict.txt
	./hashdumper dict.txt hashset.bin

hashset.o: hashset.bin hashdumper
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

level0: level0.c hashdumper hashset.o
	gcc -march=native -O3 -g -o $@ hashset.o $<

clean:
	rm -rf *.data* level0 hashdumper hashset.o hashset.bin *.o *.dSYM

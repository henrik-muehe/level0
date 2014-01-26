#!/bin/sh


# Build hash
g++ -I. -msse4.2 -std=c++11 -O0 -g -o hashdumper hashdumper.cpp
gdb -ex run -ex bt -ex quit --args ./hashdumper dict.txt hashset.bin
objcopy -I binary -O elf64-x86-64 -B i386:x86-64 hashset.bin hashset.o

# Build prober
g++ -I. -march=native -std=c++11 -O3 -g -o level0 level0.cpp hashset.o
gcc -O3 -o level0 -g -march=native level0.c hashset.o 
#!/bin/sh


# Build hash
g++ -I. -march=native -std=c++11 -O3 -g -o hashdumper hashdumper.cpp
./hashdumper dict.txt hashset.bin
objcopy -I binary -O elf64-x86-64 -B i386:x86-64 hashset.bin hashset.o

# Build prober
g++ -I. -march=native -std=c++11 -O3 -g -o level0 level0.cpp hashset.o

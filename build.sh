#!/bin/sh

# Add any build steps you need here

g++ -I. -march=native -std=c++11 -O3 -g -o level0 level0.cpp
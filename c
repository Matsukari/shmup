#!/usr/bin/bash
#COMPILE AND ASSEMBLE

SRC="src/"
OUT="src/obj/"

#m DONT ADD FORMAT; AUTO CPP
echo "g++ -std=c++17 -c $SRC$1.cpp -o $OUT$1.o"
g++ -std=c++17 -c $SRC$1.cpp -o $OUT$1.o
#!/bin/bash

npoints=200

g++ getfunc.cpp -O3 -DFIRST -o func.out
./func.out 0 5 $npoints seq1.txt

g++ getfunc.cpp -O3 -DSECOND -o func.out
./func.out 0 1 $npoints seq2.txt

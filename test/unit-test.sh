#!/bin/bash
#-----------------------------------------------------#
c++ -std=gnu++11 0-sub-sequence-string.cc -o test1
./test1
sleep 5
#-----------------------------------------------------#
make clean
make
./test2 johnny depp
#-----------------------------------------------------#

rm -f *.o



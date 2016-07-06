#------------------------------------------------- C++ Compiler efinition ------------------------------------------------#
# xapian-config --cxxflags
# xapian-config --libs
# c++ `xapian-config --cxxflags` main.cc `xapian-config --libs` -o search.cgi
#-------------------------------------------------------------------------------------------------------------------------#
CC=c++
PROGRAM=test2
#------------------------------------- Options for Compiling objects with warning level ----------------------------------#
# CFLAGS=-c -Wall -ansi -std=gnu++11 
CFLAGS	= -c -O2 -Wall -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -std=gnu++11 

INCLUDES=-I/usr/local/include -I/usr/local/lib/ -I/usr/local/lib/libxapian.so.22.7.1
LIBS=-lxapian

all: searcher
#------------------------------------- Compile all objects then linked them together --------------------------------------#
FILES=1-xapian-search.o
searcher: $(FILES) 
	$(CC) $(INCLUDES) $(LIBS) $(FILES) -o $(PROGRAM) 
	
#--------------------------------------------------------------------------------------------------------------------------#

main.o: 1-xapian-search.cc
	$(CC) $(CFLAGS) 1-xapian-search.cc 


clean:
	rm -rf *o $(PROGRAM)

#--------------------------------------------------------------------------------------------------------------------------#

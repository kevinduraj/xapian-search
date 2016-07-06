#-------------------------------------------- C++ Compiler Definition ------------------------------------------#
# xapian-config --cxxflags
# xapian-config --libs
# c++ `xapian-config --cxxflags` main.cc `xapian-config --libs` -o search.cgi
# cp /usr/local/lib/libxapian.so.30 /usr/lib64/
#---------------------------------------------------------------------------------------------------------------#
CC=c++
PROGRAM=search.cgi
#------------------------------------- Options for Compiling objects with warning level ------------------------#
# CFLAGS=-c -Wall -ansi -std=gnu++11 
CFLAGS	= -c -O2 -Wall -std=c++0x -std=gnu++11 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -I/usr/local/boost_1_61_0 
          
INCLUDES=-I/usr/local/include -I/usr/local/lib/ -I/usr/local/lib/libxapian.so.30
LIBS=-L/usr/local/lib -lxapian

all: searcher
#------------------------------------- Compile all objects then linked them together ---------------------------#
FILES=main.o Parser.o WebPage.o 
searcher: $(FILES) 
	$(CC) $(INCLUDES) $(LIBS) $(FILES) -o $(PROGRAM) 
	cp $(PROGRAM) ../bin/		
	rm -rf *o $(PROGRAM)
#---------------------------------------------------------------------------------------------------------------#

main.o: main.cc
	$(CC) $(CFLAGS) main.cc

parser.o: Parser.cpp
	$(CC) $(CFLAGS) Parser.cpp

WebPage.o: WebPage.cpp
	$(CC) $(CFLAGS) WebPage.cpp

install: 
	cp -r search.cgi ../bin/

clean:
	rm -rf *o $(PROGRAM)

#----------------------------------------------------------------------------------------------------------------#

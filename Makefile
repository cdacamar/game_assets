#********************************
#	Compiler Options        *
#********************************
CPP=g++
CFLAGS=-Wall -g -std=c++1y -I./

#********************************
#       Directives              *
#********************************

all: main

main: main.cpp
	$(CPP) $(CFLAGS) -o test main.cpp

run: test
	./test

clean: 
	rm -f test

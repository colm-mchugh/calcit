CFLAGS := -ggdb
CPP := g++

all: parser

clean:
	rm parser.cpp parser.hpp parser tokens.cpp

parser.cpp: parser.y
	bison -d -o $@ $^

parser.hpp:	parser.cpp

tokens.cpp:	tokens.l parser.hpp
	flex -o $@ $^

parser: parser.cpp nodes.cpp list.cpp main.cpp tokens.cpp analyzer.cpp context.cpp hashtable.cpp stack.cpp
	$(CPP) -o $@ $(CFLAGS) *.cpp

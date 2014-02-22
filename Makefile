target := parser
C_SRCS := parser.cpp nodes.cpp list.cpp main.cpp tokens.cpp analyzer.cpp context.cpp hashtable.cpp read_line.cpp parser_wrapper.cpp evaluater.cpp shell.cpp
C_OBJS := ${C_SRCS:.cpp=.o}
C_FLAGS := -ggdb
CPP := g++

all: $(target)

parser.cpp: parser.y
	bison -d -o $@ $^

parser.hpp:	parser.cpp

tokens.cpp:	tokens.l parser.hpp
	flex -R --header-file=lex.h -o $@ $^

$(C_OBJS): $(C_SRCS)
	$(CPP) $(C_FLAGS) -c $^

$(target): $(C_OBJS)
	$(CPP) -o $@ $(C_FLAGS) $(C_OBJS)

clean:
	rm -rf parser.cpp parser.hpp tokens.cpp lex.h
	rm -rf $(target)
	rm -rf $(C_OBJS)

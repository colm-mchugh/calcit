calcit - A simple command line interpreter for calculator expressions

1) To build and run: type make at the command-line, followed by ./calcit :
   $ make
   $ ./calcit

   Note: built and tested on ubuntu 12.04.

2) About: 'calcit' is a simple command line interpreter for evaluating calculator 
expressions. You can enter any arbitrary arithmetic expression involving the 
operators '+', '-', '*' and '/'. An expression can be saved in a variable and 
used later:

   > pi := 3.141
   3.141000
   > r := 12
   12
   > pi*r*r
   452.304016
   > 
   
   Parentheses can be used to group sub-expressions:
   >  7 * 3 + 4
   25
   > 7 * (3 + 4)
   49
   
   Integer and float constants can be used in the same expression: 
   > 1 + 1/2.0
   1.500000
  
   See calcit.example for examples. To run these examples, enter:
   $ ./calcit < calcit.example

3) Code details:
   
The shell (shell.cpp) is the driver; it waits for input and then processes input 
using the following modules:

(1) parser (parser_wrapper.h, parser.y, tokens.l)
	Input: text string
	Output: node tree (node.h)
(2) analyzer (analyzer.h)
	Input: node tree
	Output: analyzed node tree and updated symbol table, or list of errors
(3) evaluater (evaluater.h)
	Input: analyzed node tree
	Output: evaluated expression (ExprValue in context.h). 
A context object (context.h) is used to hold state for processing input (node tree, 
symbol table, evaluation functions, error list).
	


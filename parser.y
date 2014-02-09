%{
#include "nodes.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List *parse_list;

extern int yylex();
void yyerror(const char *s) { printf("ERROR: %s\n", s); }
%}

%union {
	List *list;
	Node *node;
	AssignNode *assign_node;
	char string[64];
	int token;
}

%token <string> T_INT T_FLT T_IDENTIFIER
%token <token> T_ASSIGNMENT T_MINUS T_PLUS T_MULT
%token <token> T_DIV T_LPAREN T_RPAREN T_SEMICOLON

/* Define the type of non-terminal symbols.
   Associate a union variable with one or more
   non-terminal symbols
   */
%type <list> program assignments
%type <assign_node> assignment
%type <node> expr 
%type <node> number
 
%left T_PLUS T_MINUS
%left T_MULT T_DIV

%start program

%%

program: assignments expr
		{ 
		  appendTo($1, $2); 
		  $$ = $1;
		  parse_list = $$; 
		  printf("parsed program\n");
		}
		;

assignments:	{ $$ = createList(); }
			|	assignments assignment 
				{ 
				  appendTo($1, $2);
				  $$ = $1;
				  printf("parsed assignments\n");
				}
			;
				
assignment: T_IDENTIFIER T_ASSIGNMENT expr T_SEMICOLON 
			{ $$ = makeAssignNode(makeIdentNode($1), $3); }
		;

expr	:	expr T_PLUS expr
			{ $$ = (Node*)makeExprNode('+', $1, $3); }
		|   expr T_MINUS expr		
			{ $$ = (Node*)makeExprNode('-', $1, $3); }
		|   expr T_MULT	 expr 
			{ $$ = (Node*)makeExprNode('*', $1, $3); }
		|   expr T_DIV	 expr	
			{ $$ = (Node*)makeExprNode('/', $1, $3); }
		|	T_LPAREN expr T_RPAREN
			{ $$ = (Node*)$2; }
		| 	T_IDENTIFIER
			{ $$ = (Node*)makeIdentNode($1); }
		|	number 
		;

number	: T_INT 
		{ $$ = (Node*)makeIntNode(atoi($1)); }
		| T_FLT 
		{ $$ = (Node*)makeFltNode(atof($1)); }

		;
%%

%define api.pure
%lex-param { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { void *parse_tree }
%{
#include "nodes.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.hpp"
#include "lex.h"


#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

void yyerror(yyscan_t scanner, void *parse_tree, const char *s) {
	List *l = (List*)parse_tree;
	SyntaxErrNode *err = makeSyntaxErrNode(s);
	appendTo(l, err);
}

%}

%union {
	List *list;
	Node *node;
	AssignNode *assign_node;
	char string[MAX_TOKEN_LEN];
	int token;
}

%start program
%token <string> T_INT T_FLT T_IDENTIFIER
%token <token> T_ASSIGNMENT T_MINUS T_PLUS T_MULT
%token <token> T_DIV T_LPAREN T_RPAREN T_SEMICOLON

/* Define the type of non-terminal symbols.
   Associate a union variable with one or more
   non-terminal symbols
   */
%type <list> program
%type <assign_node> assignment
%type <node> expr 
%type <node> number
 
%left T_PLUS T_MINUS
%left T_MULT T_DIV

%%

program: assignment  { appendTo((List*)parse_tree, $1); $$ = (List*)parse_tree; }
		| expr  { appendTo((List*)parse_tree, $1); $$ = (List*)parse_tree; }
		;

assignment: T_IDENTIFIER T_ASSIGNMENT expr
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
		|	T_MINUS expr
			{ $$ = (Node*)makeExprNode('*', (Node*)makeIntNode("-1"), (Node*)$2); }
		;

number	: T_INT 
		{ $$ = (Node*)makeIntNode($1); }
		| T_FLT 
		{ $$ = (Node*)makeFltNode($1); }

		;
%%

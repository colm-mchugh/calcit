%define api.pure
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }
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
%token <token> T_DIV T_LPAREN T_RPAREN T_SEMICOLON T_COMMA
%token <token> T_LET T_IN
%token <token> T_IF T_THEN T_ELSE
%token <token> T_FOR
%token <token> T_DEF
%token <token> T_EQ T_NE T_LT T_GT T_LTE T_GTE

/* Define the type of non-terminal symbols.
   Associate a union variable with one or more
   non-terminal symbols
   */
%type <list> program
%type <node> expr 
%type <node> tl_expr 
%type <node> number opt_init else_part

 
%right T_ASSIGNMENT 
%left T_LT T_GT T_LTE T_GTE T_EQ T_NE 
%left T_PLUS T_MINUS
%left T_MULT T_DIV

%%

program:  tl_expr  { appendTo((List*)parse_tree, $1); $$ = (List*)parse_tree; }
		;

tl_expr	:	T_LET declist T_IN tl_expr 
		|	T_FOR T_IDENTIFIER T_ASSIGNMENT expr T_COMMA expr T_COMMA expr
			T_IN tl_expr
		|	T_IF expr T_THEN tl_expr else_part
		|	T_IDENTIFIER T_LPAREN args T_RPAREN
			{ /* TBD: makeCallNode($1, $3) */ } 
		|   T_IDENTIFIER T_ASSIGNMENT expr
            { /* TBD: $$ = makeAssignNode(makeIdentNode($1), $3); */  }
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
		|   T_IDENTIFIER T_LPAREN args T_RPAREN
		|	T_MINUS expr
			{ $$ = (Node*)makeExprNode('*', (Node*)makeIntNode("-1"), (Node*)$2); }
		;

number	: T_INT 
		{ $$ = (Node*)makeIntNode($1); }
		| T_FLT 
		{ $$ = (Node*)makeFltNode($1); }
		;

else_part	: T_ELSE tl_expr
			{ $$ = $2; }
			| { $$ = NULL; }
			;
			
args	: idlist 
		|
		;

idlist	: idlist T_COMMA T_IDENTIFIER
		| T_IDENTIFIER
		;		

declist	: declist T_COMMA T_IDENTIFIER opt_init
		| T_IDENTIFIER opt_init
		;
		
opt_init	: T_ASSIGNMENT expr
			{ $$ = $2; }
			| { $$ = NULL; }
			;
					
%%

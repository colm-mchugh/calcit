%define api.pure
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }
%parse-param { void *parse_tree }
%{
//#include "nodes.h"
#include "list.h"
#include "expr.h"
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
	SyntaxErrNode *err = new SyntaxErrNode(s);
	appendTo(l, err);
}

%}

%union {
	expr *exp;
	char ident[MAX_TOKEN_LEN];
	int token;
	List *slist;
}

%start program
%token <ident> T_INT T_FLT T_IDENTIFIER T_TRUE T_FALSE
%token <token> T_ASSIGNMENT T_MINUS T_PLUS T_MULT
%token <token> T_DIV T_MOD T_LPAREN T_RPAREN T_SEMICOLON T_COMMA T_PIPE
%token <token> T_LET T_IN
%token <token> T_IF T_THEN T_ELSE
%token <token> T_FOR
%token <token> T_DEF
%token <token> T_WHILE T_ENDO T_ENDIF T_ENDEF
%token <token> T_CONTINUE
%token <token> T_BREAK
%token <token> T_RETURN
%token <token> T_DO
%token <token> T_EQ T_NE T_LT T_GT T_LTE T_GTE
%token <token> T_OR T_AND T_NOT

/* Define the type of non-terminal symbols.
   Associate a union variable with one or more
   non-terminal symbols
   */
%type <exp> program compound_s simple_s stmt
%type <slist>  e_list opt_args idlist params
%type <exp> suite if_s  while_s funcdef 
%type <exp>  test expr_s  factor  atom pow
%type <exp> number flow_s    
 
%right T_ASSIGNMENT 
%left T_OR
%left T_AND
%left T_EQ T_NE
%left T_LT T_GT T_LTE T_GTE  
%left T_PLUS T_MINUS
%left T_MULT T_DIV T_MOD
%left UNARY

%{
	expr_opcode tokToOpcode(int token) {
		switch(token) {
		   case T_EQ : return expr_opcode::OP_EQ;
		   case T_NE : return expr_opcode::OP_NE;
		   case T_GT : return expr_opcode::OP_GT;
		   case T_LT : return expr_opcode::OP_LT;
		   case T_GTE : return expr_opcode::OP_GTE;
		   case T_LTE : return expr_opcode::OP_LTE;
		}
		return expr_opcode::OP_UNKNOWN;
	}
%}

%%

program:  stmt  
		;

suite:	stmt 
		| suite stmt 
		;
				
stmt:	simple_s
		| compound_s
		| %empty { $$ = NULL; }
		;
				
compound_s:	if_s
			| while_s
			| funcdef
//			| with_s
			;
 
if_s:	T_IF test T_THEN suite T_ENDIF
			{ $$ = new If(std::unique_ptr<expr>($2), std::unique_ptr<expr>($4), std::unique_ptr<expr>(nullptr)); }
	   | T_IF test T_THEN suite T_ELSE suite T_ENDIF
			{ $$ = new If(std::unique_ptr<expr>($2), std::unique_ptr<expr>($4), std::unique_ptr<expr>($6)); }
		;
		
while_s:	T_WHILE test T_DO suite T_ENDO
				{ $$ = new While(std::unique_ptr<expr>($2), std::unique_ptr<expr>($4)); }
			;
			
funcdef:	T_DEF T_IDENTIFIER T_LPAREN params T_RPAREN suite T_ENDEF
				{ VarRef *n = new VarRef($2);
				  $$ = new Fnctn(std::unique_ptr<VarRef>(n), $4, std::unique_ptr<expr>($6));
				}
			;

params	: idlist 
		| %empty { $$ = NULL; }
		;

idlist	: idlist T_COMMA T_IDENTIFIER
			{ appendTo($$, new VarRef($3));
			}
		| T_IDENTIFIER
			{ $$ = createList();
			  appendTo($$, new VarRef($1)); 
			}
		;
			
simple_s:	expr_s
			| flow_s
			;
			
expr_s:		test T_ASSIGNMENT test
				{ $$ = new Vardef(std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
			| test
			;

flow_s:		T_BREAK
				{ $$ = new Break(); }
			| T_CONTINUE
				{ $$ = new Continue(); }
			| T_RETURN
				{ $$ = new Return(std::unique_ptr<expr>(nullptr)); }
			| T_RETURN test
				{ $$ = new Return(std::unique_ptr<expr>($2)); }
			;


test:	  test T_OR test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_AND test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_LT test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_GT test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_EQ test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_NE test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_GTE test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_LTE test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_PLUS test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_MINUS test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_MULT test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_DIV test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| test T_MOD test { $$ = new BinaryOp(tokToOpcode($2), std::unique_ptr<expr>($1), std::unique_ptr<expr>($3)); }
		| factor
		;
			
factor:		T_MINUS factor %prec UNARY
				{ $$ = new UnaryOp(tokToOpcode($1), std::unique_ptr<expr>($2)); }
			| T_PLUS factor %prec UNARY
				{ $$ = new UnaryOp(tokToOpcode($1), std::unique_ptr<expr>($2)); }
			| T_NOT factor %prec UNARY
				{ $$ = new UnaryOp(tokToOpcode($1), std::unique_ptr<expr>($2)); }
			| pow
			;
			
pow:		atom T_LPAREN opt_args T_RPAREN
				{ $$ = new Fcall(std::unique_ptr<expr>($1), $3);	} 
			| atom
			;			

opt_args:	e_list
			| %empty { $$ = createList(); }
			;

e_list:		e_list T_COMMA test
				{ appendTo($$, $3); }
			| test
				{ $$ = createList();
			  	  appendTo($$, $1);
				}
			;

atom:		T_IDENTIFIER
			{ $$ = new VarRef($1); }
			| number
			| T_LPAREN test T_RPAREN
			{ $$ = $2; }
			| T_TRUE 
			  { $$ = new BoolLiteral(true); }
			| T_FALSE
			  { $$ = new BoolLiteral(false); }
			;

number	: T_INT 
		{ $$ = new IntLiteral($1); }
		| T_FLT 
		{ $$ = new FltLiteral($1); }
		;
						
%%

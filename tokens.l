%option reentrant bison-bridge noyywrap
%{
#include <string.h>
#include "nodes.h"
#include "list.h"
#include "parser.hpp"
#define SAVE_TOKEN strcpy(yylval->string, yytext)
#define TOKEN(t) (yylval->token = t) 
%}

%%

[ \t\n]			;
[a-zA-Z_][a-zA-Z_0-9]*	SAVE_TOKEN; return T_IDENTIFIER;
[-+]?[0-9]+					SAVE_TOKEN; return T_INT;
[-+]?[0-9]+\.[0-9]*			SAVE_TOKEN; return T_FLT;
":="					return TOKEN(T_ASSIGNMENT);
"-"						return TOKEN(T_MINUS);
"+"						return TOKEN(T_PLUS);
"*"						return TOKEN(T_MULT);
"/" 					return TOKEN(T_DIV);
"(" 					return TOKEN(T_LPAREN);
")" 					return TOKEN(T_RPAREN);
";" 					return TOKEN(T_SEMICOLON);

%%

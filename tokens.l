%option reentrant bison-bridge noyywrap
%{
#include <string.h>
#include "nodes.h"
#include "list.h"
#include "parser.hpp"
#define SAVE_TOKEN strncpy(yylval->string, yytext, MAX_TOKEN_LEN); \
					yylval->string[MAX_TOKEN_LEN-1] = '\0'
#define TOKEN(t) (yylval->token = t) 
%}

%%

[ \t\n]			;
[Ll][Ee][Tt]			return TOKEN(T_LET);
[Ii][Nn]				return TOKEN(T_IN);
[Ii][Ff]				return TOKEN(T_IF);
[Tt][Hh][Ee][Nn]		return TOKEN(T_THEN);
[Ee][Ll][Ss][Ee]		return TOKEN(T_ELSE);
[Ff][Oo][Rr]			return TOKEN(T_FOR);
[Dd][Ee][Ff]			return TOKEN(T_DEF);
[a-zA-Z_][a-zA-Z_0-9]*	SAVE_TOKEN; return T_IDENTIFIER;
[0-9]+					SAVE_TOKEN; return T_INT;
[0-9]+\.[0-9]*			SAVE_TOKEN; return T_FLT;
":="					return TOKEN(T_ASSIGNMENT);
"-"						return TOKEN(T_MINUS);
"+"						return TOKEN(T_PLUS);
"*"						return TOKEN(T_MULT);
"/" 					return TOKEN(T_DIV);
"(" 					return TOKEN(T_LPAREN);
")" 					return TOKEN(T_RPAREN);
","						return TOKEN(T_COMMA);
"=="					return TOKEN(T_EQ);
"!="					return TOKEN(T_NE);
"<"						return TOKEN(T_LT);
">"						return TOKEN(T_GT);
"<="					return TOKEN(T_LTE);
">="					return TOKEN(T_GTE);
";" 					return TOKEN(T_SEMICOLON);

%%

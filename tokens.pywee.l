%option reentrant bison-bridge noyywrap
%{
#include <string.h>
#include "nodes.h"
#include "list.h"
#include "parser.hpp"
#define SAVE_TOKEN strncpy(yylval->ident, yytext, MAX_TOKEN_LEN); \
					yylval->ident[MAX_TOKEN_LEN-1] = '\0'
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
[Dd][Oo]				return TOKEN(T_DO);
[Oo][Rr]				return TOKEN(T_OR);
[Aa][Nn][Dd]			return TOKEN(T_AND);
[Nn][Oo][Tt]			return TOKEN(T_NOT);
[Ww][Hh][Ii][Ll][Ee]	return TOKEN(T_WHILE);
[Cc][Oo][Nn][Tt][Ii][Nn][Uu][Ee]		return TOKEN(T_CONTINUE);
[Bb][Rr][Ee][Aa][Kk]	return TOKEN(T_BREAK);
[Rr][Ee][Tt][Uu][Rr][Nn]			return TOKEN(T_RETURN);
[Tt][Rr][Uu][Ee]		return TOKEN(T_TRUE);
[Ff][Aa][Ll][Ss][Ee]	return TOKEN(T_FALSE);
[Ee][Nn][Dd][Ee][Ff]	return TOKEN(T_ENDEF);
[Ee][Nn][Dd][Ii][Ff]	return TOKEN(T_ENDIF);
[Ee][Nn][Dd][Oo]		return TOKEN(T_ENDO);			
[a-zA-Z_][a-zA-Z_0-9]*	SAVE_TOKEN; return T_IDENTIFIER;
[0-9]+					SAVE_TOKEN; return T_INT;
[0-9]+\.[0-9]*			SAVE_TOKEN; return T_FLT;
":="					return TOKEN(T_ASSIGNMENT);
"-"						return TOKEN(T_MINUS);
"+"						return TOKEN(T_PLUS);
"*"						return TOKEN(T_MULT);
"/" 					return TOKEN(T_DIV);
"%"						return TOKEN(T_MOD);
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
"|"						return TOKEN(T_PIPE);

%%

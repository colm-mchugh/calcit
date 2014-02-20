#include "parser_wrapper.h"
#include "nodes.h"
#include "parser.hpp"
#include "lex.h"

extern int yyparse (yyscan_t scanner, void *parse_tree_ptr);

List *parse_str(char *str)
{
	yyscan_t scanner;
	List *parse_tree = createList();
	if (yylex_init(&scanner)) {
		//TODO: handle failure in initializing lexer
	}
	YY_BUFFER_STATE yybs = yy_scan_string(str, scanner);
	yyparse(scanner, (void*)parse_tree);
	yy_delete_buffer(yybs, scanner);
	yylex_destroy(scanner);
	return parse_tree;
}

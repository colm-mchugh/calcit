#include "parser_wrapper.h"
#include "lex.h"

extern List* parse_list;
typedef yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_buffer(char *, size_t);

List *parse_str(char *str)
{
	int input_len = strlen(str);
	char *parse_input = (char*)malloc(input_len+3);
	strcpy(parse_input, str);
	parse_input[input_len] = '\0';
	parse_input[input_len+1] = '\0';
	YY_BUFFER_STATE yybs = yy_scan_buffer(parse_input, strlen(str)+2);
	yy_switch_to_buffer(yybs);
	yyparse();
	yy_delete_buffer(yybs);
	free(parse_input); //needed ??
	return parse_list;
}

#include "list.h"
#include "nodes.h"
#include "context.h"
#include "analyzer.h"
#include "evaluater.h"
#include <stdio.h>
#include "lex.h"
#include "read_line.h"

extern List* parse_list;
typedef yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_buffer(char *, size_t);

int main(int argc, char **argv)
{
	char *input = argv[0];
	Context *parse_context = createContext();
	while(1) {
		printf("Enter> ");
		input = read_line(stdin);
		if(strcmp(input, "q") == 0) {
			break;
		}
		YY_BUFFER_STATE yybs = yy_scan_buffer(input, strlen(input)+2);
		yy_switch_to_buffer(yybs);
		yyparse();
		yy_delete_buffer(yybs);
		parse_context->parse_tree = parse_list;
		analyzeParseTree(parse_context);
		if (!is_empty(parse_context->errors)) {
			while(!is_empty(parse_context->errors)) {
				ErrorMessage *em = (ErrorMessage*)pop(parse_context->errors);
				printf("Error: %s\n", em->message);
				appendTo(parse_context->to_delete, em);
			}
		} else {
			List *parse_tree = parse_context->parse_tree;
			Node *node = (Node*)getElement(parse_tree, 0);
			ExprValue val = evaluateNode(node, parse_context);
			switch (val.type) {
				case T_INT_CONST: printf("%d\n", val.value.int_value); break;
				case T_FLT_CONST: printf("%lf\n", val.value.float_value); break;
			}
		}
	}
	return 0;
}

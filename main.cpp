#include "parser_wrapper.h"
#include "nodes.h"
#include "context.h"
#include "analyzer.h"
#include "evaluater.h"
#include <stdio.h>
#include "lex.h"
#include "read_line.h"

int main(int argc, char **argv)
{
	char *input = argv[0];
	Context *parse_context = createContext();
	while(1) {
		printf("Enter> ");
		input = read_line(stdin);
		if(strcmp(input, "") == 0) {
			continue;
		}
		if(strcmp(input, "q") == 0) {
			break;
		}
		parse_context->parse_tree = parse_str(input);
		free_line(&input);

		analyzeParseTree(parse_context);
		if (!is_empty(parse_context->errors)) {
			while(!is_empty(parse_context->errors)) {
				ErrorMessage *em = (ErrorMessage*)pop(parse_context->errors);
				printf("%s\n", em->message);
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
		cleanUpContext(parse_context);
	}
	return 0;
}

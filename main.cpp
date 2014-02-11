#include "list.h"
#include "nodes.h"
#include "context.h"
#include "analyzer.h"
#include "evaluater.h"

#include <stdio.h>

extern List* parse_list;
extern int yyparse();

int main(int argc, char **argv)
{
	yyparse();
	int num_nodes = list_size(parse_list);
	for(int i = 0; i < num_nodes; i++) {
		printf("Node at index %d:\t", i);
		displayNode((Node*)getElement(parse_list, i));
		printf("\n");
	}
	Context *parse_context = createContext();
	parse_context->parse_tree = parse_list;
	analyzeParseTree(parse_context);

	List *parse_tree = parse_context->parse_tree;
	for (int i = 0; i < num_nodes; i++) {
		Node *node = (Node*)getElement(parse_tree, i);
		ExprValue val = evaluateNode(node, parse_context);
		switch (val.type) {
			case T_INT_CONST: printf("Expr Value=%d\n", val.value.int_value); break;
			case T_FLT_CONST: printf("Expr value=%lf\n", val.value.float_value); break;
		}
	}

	return 0;
}

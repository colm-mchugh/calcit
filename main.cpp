#include "list.h"
#include "nodes.h"
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
	return 0;
}

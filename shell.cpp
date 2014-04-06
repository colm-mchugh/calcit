#include "parser_wrapper.h"
#include "nodes.h"
#include "context.h"
#include "analyzer.h"
#include "evaluater.h"
#include <stdio.h>
#include <string.h>
#include "read_line.h"

#define PROMPT "> "

// top-level driver function: 
int run_shell(FILE *input, FILE *output, FILE *error)
{
	char *code_text ;
	Context *ctx = createContext();
	while(1) {
		fprintf(output, PROMPT);
		code_text = read_line(input);
		if(strcmp(code_text, "") == 0) {
			continue;
		}
		if(strcmp(code_text, "q") == 0) {
			break;
		}
		// parse the input into a node tree:
		ctx->parse_tree = parse_str(code_text);
		free_line(&code_text);

		// Analyze the parsed tree for consistency (resolve variable uses):
		analyzeParseTree(ctx);
		if (list_size(ctx->errors) > 0) {
			// Report any errors from analyze:
			for(int i = 0; i < list_size(ctx->errors); i++) {
				ErrorMessage *em = (ErrorMessage*)getElement(ctx->errors, i);
				fprintf(error, "%s\n", em->message);
			}
		} else {
			// If no errors during analyze, evaluate the node tree to it's value
			List *parse_tree = ctx->parse_tree;
			Node *node = (Node*)getElement(parse_tree, 0);
			ExprValue val = evaluateNode(node, ctx);
			switch (val.type) {
				case T_INT_CONST: fprintf(output, "%d\n", val.value.int_value); break;
				case T_FLT_CONST: fprintf(output, "%lf\n", val.value.float_value); break;
			}
		}
		// Free any resources (nodes, errors)
		cleanUpContext(ctx);
	}
	// End of interaction: free resources (last line entered, context) 
	free_line(&code_text);
	deleteContext(&ctx);
	return 0;
}

#include "parser_wrapper.h"
#include "nodes.h"
#include "context.h"
#include "analyzer.h"
#include "evaluater.h"
#include <stdio.h>
#include <string.h>
#include "read_line.h"

int run_shell(FILE *input, FILE *output, FILE *error)
{
	char *code_text ;
	Context *ctx = createContext();
	while(1) {
		fprintf(output, "Enter> ");
		code_text = read_line(input);
		if(strcmp(code_text, "") == 0) {
			continue;
		}
		if(strcmp(code_text, "q") == 0) {
			break;
		}
		ctx->parse_tree = parse_str(code_text);
		free_line(&code_text);

		analyzeParseTree(ctx);
		if (list_size(ctx->errors) > 0) {
			for(int i = 0; i < list_size(ctx->errors); i++) {
				ErrorMessage *em = (ErrorMessage*)getElement(ctx->errors, i);
				fprintf(error, "%s\n", em->message);
			}
		} else {
			List *parse_tree = ctx->parse_tree;
			Node *node = (Node*)getElement(parse_tree, 0);
			ExprValue val = evaluateNode(node, ctx);
			switch (val.type) {
				case T_INT_CONST: fprintf(output, "%d\n", val.value.int_value); break;
				case T_FLT_CONST: fprintf(output, "%lf\n", val.value.float_value); break;
			}
		}
		cleanUpContext(ctx);
	}
	free_line(&code_text);
	deleteContext(&ctx);
	return 0;
}

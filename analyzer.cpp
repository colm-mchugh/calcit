#include <string.h>
#include "analyzer.h"
#include "nodes.h"

void analyzeNode(Node *, Context *); 

void analyzeExprNode(ExprNode *node, Context *ctx) {
	analyzeNode(node->left, ctx);
	analyzeNode(node->right, ctx);
}

bool compare_fn(void *arg1, void *arg2) {
	AssignNode *node = (AssignNode*)arg1;
	char *key = (char*)arg2;
	return strcmp(node->target->identifier, key) == 0;
}

void analyzeAssignNode(AssignNode *node, Context *ctx) {
	compare cf = compare_fn;
	AssignNode *an = (AssignNode*)lookup(ctx->symbol_table, node->target->identifier, cf);
	// Analyze the RHS of the assignment:
	int error_count = list_size(ctx->errors);
	if (node->value != NULL) {
		analyzeNode(node->value, ctx);
	}
	if (list_size(ctx->errors) == error_count) {
		// If there are no errors from analyze of RHS, remove existing assignment and add new assignment 
		if (an != NULL) { 
			remove(ctx->symbol_table, node->target->identifier, an);
			deleteNode((Node*)an);
		}
		add(ctx->symbol_table, node->target->identifier, node);
	}
}


void analyzeIdentNode(IdentNode *node, Context *ctx) {
	AssignNode *an = (AssignNode*)lookup(ctx->symbol_table, node->identifier, compare_fn);
	if (an == NULL) {
		prependTo(ctx->errors, makeError(NO_SUCH_IDENT, node->identifier));
	} else {
		// TODO: it may not be necessary to resolve ident every time.
		node->declaration = (Node*)an;
	}
}


void analyzeNode(Node *node, Context *ctx) {
	switch (node->type) {
	case T_ASSIGN: 
			analyzeAssignNode((AssignNode*)node, ctx);
				break;
	case T_EXPR: 
			analyzeExprNode((ExprNode*)node, ctx);
				break;
	case T_IDENT: 
			analyzeIdentNode((IdentNode*)node, ctx);
				break;
	case T_SYNTAX_ERROR: 
			prependTo(ctx->errors, makeError(SYNTAX_ERROR, ((SyntaxErrNode*)node)->error_message));
				break;
	}
}

void analyzeParseTree(Context* ctx) {
	List *parse_tree = ctx->parse_tree;
	int num_nodes = list_size(parse_tree);
	for (int i = 0; i < num_nodes; i++) {
		analyzeNode((Node*)getElement(parse_tree, i), ctx);
	}
}

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
	if (an != NULL) {
		// TODO: Remove an from symbol table - there is a newer assignment which overwrites it
		// requires remove(List*, void*)
	}
	add(ctx->symbol_table, node->target->identifier, node);
}


void analyzeIdentNode(IdentNode *node, Context *ctx) {
	AssignNode *an = (AssignNode*)lookup(ctx->symbol_table, node->identifier, compare_fn);
	if (an == NULL) {
		push(ctx->errors, makeError(NO_SUCH_IDENT, node->identifier));
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
	}
}

void analyzeParseTree(Context* ctx) {
	List *parse_tree = ctx->parse_tree;
	int num_nodes = list_size(parse_tree);
	for (int i = 0; i < num_nodes; i++) {
		analyzeNode((Node*)getElement(parse_tree, i), ctx);
	}
}

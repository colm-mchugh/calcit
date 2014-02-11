#include "evaluater.h"
#include "context.h"
#include <stdlib.h>
#include <string.h>

ExprValue evaluateNode(Node*, Context*);

ExprValue evaluateExprNode(ExprNode *node, Context *ctx) {
	ExprValue left = evaluateNode(node->left, ctx);
	ExprValue right = evaluateNode(node->right, ctx);
	ExprValue val = doEval(ctx, left, right, node->opcode);
	return val;
}

ExprValue evaluateIdentNode(IdentNode *node, Context *ctx) {
	AssignNode *an = (AssignNode*)node->declaration;
	ExprValue result = *((ExprValue*)an->resolved_value);
	return result;
}

ExprValue evaluateAssignNode(AssignNode *node, Context *ctx) {
	ExprValue result = evaluateNode(node->value, ctx);
	node->resolved_value = malloc(sizeof(ExprValue));
	memcpy(node->resolved_value, &result, sizeof(ExprValue));
	return result;
}

ExprValue evaluateIntNode(IntNode *node, Context *ctx) {
	ExprValue result;
	result.type = node->type;
	result.value.int_value = node->value;
	return result;
}

ExprValue evaluateFltNode(FltNode *node, Context *ctx) {
	ExprValue result;
	result.type = node->type;
	result.value.float_value = node->value;
	return result;
}

ExprValue evaluateNode(Node *node, Context *ctx) {
	switch (node->type) {
		case T_INT_CONST: return evaluateIntNode((IntNode*)node, ctx); break;
		case T_FLT_CONST: return evaluateFltNode((FltNode*)node, ctx); break;
		case T_IDENT: return evaluateIdentNode((IdentNode*)node, ctx); break;
		case T_EXPR: return evaluateExprNode((ExprNode*)node, ctx); break;
		case T_ASSIGN: return  evaluateAssignNode((AssignNode*)node, ctx); break;
	}
}

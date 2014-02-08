#include "nodes.h"
#include <string.h>
#include <stdlib.h>

Node *allocateNodeMemory(NodeTag node_tag) {
	Node *new_node;
	switch (node_tag) {
		case T_ASSIGN: 
				new_node = (Node*)malloc(sizeof(AssignNode));
					break;
		case T_EXPR: 
				new_node = (Node*)malloc(sizeof(ExprNode));
					break;
		case T_IDENT: 
				new_node = (Node*)malloc(sizeof(IdentNode));
					break;
		case T_FLT_CONST: 
				new_node = (Node*)malloc(sizeof(FltNode));
					break;
		case T_INT_CONST: 
				new_node = (Node*)malloc(sizeof(IntNode));
					break;
	}
	new_node->type = node_tag;
	return new_node;
}

void freeNodeMemory(Node **node_ptr) {
	if (node_ptr != NULL && *node_ptr != NULL) {
			free(*node_ptr);
			*node_ptr = NULL;
	}
}

IntNode *makeIntNode(int value) {
	IntNode *int_node = (IntNode*)allocateNodeMemory(T_INT_CONST);
	int_node->value = value;
	return int_node;
}

FltNode *makeFltNode(float value) {
	FltNode *flt_node = (FltNode*)allocateNodeMemory(T_FLT_CONST);
	flt_node->value = value;
	return flt_node;
}

IdentNode *makeIdentNode(char* ident) {
	IdentNode *id_node = (IdentNode*)allocateNodeMemory(T_IDENT);
	id_node->identifier = (char*)malloc(strlen(ident) + 1);
	strcpy(id_node->identifier, ident);
	return id_node;
}

ExprNode *makeExprNode(char op, Node* oprnd1, Node* oprnd2) {
	ExprNode *expr_node = (ExprNode*)allocateNodeMemory(T_EXPR);
	expr_node->left = oprnd1;
	expr_node->right = oprnd2;
	expr_node->opcode = op;
	return expr_node;
}

AssignNode *makeAssignNode(IdentNode* id, Node* val) {
	AssignNode *assign_node = (AssignNode*)allocateNodeMemory(T_ASSIGN);
	assign_node->target = id;
	assign_node->value = val;
	return assign_node;
}

// TODO Figure out best way to visit nodes
// Does this need to be a recursive function?
void displayNode(Node *node) {
	switch (node->type) {
		case T_ASSIGN: 
					break;
		case T_EXPR: 
					break;
		case T_IDENT: 
					break;
		case T_FLT_CONST: 
					break;
		case T_INT_CONST: 
					break;
	}
}

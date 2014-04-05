#include "nodes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
		case T_SYNTAX_ERROR: 
				new_node = (Node*)malloc(sizeof(SyntaxErrNode));
					break;
	}
	new_node->type = node_tag;
	return new_node;
}

void freeNodeMemory(void **node_ptr) {
	if (node_ptr != NULL && *node_ptr != NULL) {
			free(*node_ptr);
			*node_ptr = NULL;
	}
}

IntNode *makeIntNode(const char* value) {
	IntNode *int_node = (IntNode*)allocateNodeMemory(T_INT_CONST);
	int_node->value = atoi(value);
	return int_node;
}

FltNode *makeFltNode(const char* value) {
	FltNode *flt_node = (FltNode*)allocateNodeMemory(T_FLT_CONST);
	flt_node->value = atof(value);
	return flt_node;
}

IdentNode *makeIdentNode(const char* ident) {
	IdentNode *id_node = (IdentNode*)allocateNodeMemory(T_IDENT);
	int ident_len = strlen(ident) + 1;
	id_node->identifier = (char*)malloc(ident_len);
	strncpy(id_node->identifier, ident, ident_len);
	id_node->declaration = NULL;
	return id_node;
}

void deleteIdentNode(IdentNode *node) {
	free(node->identifier);
	freeNodeMemory((void**)&node);
}

ExprNode *makeExprNode(char op, Node* oprnd1, Node* oprnd2) {
	ExprNode *expr_node = (ExprNode*)allocateNodeMemory(T_EXPR);
	expr_node->left = oprnd1;
	expr_node->right = oprnd2;
	expr_node->opcode = op;
	return expr_node;
}

void deleteExprNode(ExprNode *node) {
	deleteNode(node->left);
	deleteNode(node->right);
	freeNodeMemory((void**)&node);
}

AssignNode *makeAssignNode(IdentNode* id, Node* val) {
	AssignNode *assign_node = (AssignNode*)allocateNodeMemory(T_ASSIGN);
	assign_node->target = id;
	assign_node->value = val;
	assign_node->resolved_value = NULL;
	return assign_node;
}

void deleteAssignNode(AssignNode *node) {
	if (node != NULL) {
		if (node->resolved_value != NULL) {
			free (node->resolved_value);
		}
		if (node->value != NULL) {
			deleteNode(node->value);
		}
		if (node->target != NULL) {
			deleteIdentNode(node->target);
		}
		freeNodeMemory((void**)&node);
	}
}

SyntaxErrNode *makeSyntaxErrNode(const char *error) {
	SyntaxErrNode *syntax_error = (SyntaxErrNode*)allocateNodeMemory(T_SYNTAX_ERROR);
	int msg_len = strlen(error) + 1;
	syntax_error->error_message = (char*)malloc(msg_len);
	strncpy(syntax_error->error_message, error, msg_len);
	return syntax_error;
}

void deleteSyntaxErrNode(SyntaxErrNode *node) {
	free(node->error_message);
	freeNodeMemory((void**)&node);
}


void deleteNode(Node *node) {
	switch (node->type) {
		case T_ASSIGN:
			deleteAssignNode((AssignNode*)node); break;
		case T_IDENT:
			deleteIdentNode((IdentNode*)node); break;
		case T_EXPR:
			deleteExprNode((ExprNode*)node); break;
		case T_SYNTAX_ERROR:
			deleteSyntaxErrNode((SyntaxErrNode*)node); break;
		default:
			freeNodeMemory((void**)&node); break;
	}
}
			
bool genericDeleteNode(void *node) {
	deleteNode((Node*)node);
}

// TODO Figure out best way to visit nodes
// Does this need to be a recursive function?
void displayNode(Node *node) {
	switch (node->type) {
		case T_ASSIGN: 
				displayAssignNode((AssignNode*)node);
					break;
		case T_EXPR: 
				displayExprNode((ExprNode*)node);
					break;
		case T_IDENT: 
				displayIdentNode((IdentNode*)node);
					break;
		case T_FLT_CONST: 
				displayFltNode((FltNode*)node);
					break;
		case T_INT_CONST: 
				displayIntNode((IntNode*)node);
					break;
	}
}

void displayIntNode(IntNode* node) {
	printf(" [T_INT_CONST:%d] ", node->value);
}
void displayFltNode(FltNode* node) {
	printf(" [T_FLT_CONST:%f] ", node->value);
}

void displayIdentNode(IdentNode* node) {
	printf(" [T_IDENT:%s] ", node->identifier);
}

void displayExprNode(ExprNode* node) {
	displayNode(node->left);
	displayNode(node->right);
	printf(" [T_EXPR:%c] ", node->opcode);
}

void displayAssignNode(AssignNode* node) {
	displayIdentNode(node->target);
	printf(" [T_ASSIGN] ");
	displayNode(node->value);
}

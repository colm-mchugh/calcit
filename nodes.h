#ifndef _nodes_h_
#define _nodes_h_

typedef enum NodeTag
{
	T_INT_CONST,
	T_FLT_CONST,
	T_IDENT,
	T_EXPR,
	T_ASSIGN
} NodeTag;

typedef struct Node {
	NodeTag type;
} Node;

typedef struct intConst {
	NodeTag type;
	int value;
} IntNode;

typedef struct fltConst {
	NodeTag type;
	float value;
} FltNode;

typedef struct ident {
	NodeTag type;
	char *identifier;
} IdentNode;

typedef struct expr {
	NodeTag type;
	char opcode;
	Node* left;
	Node* right;
} ExprNode;

typedef struct assign {
	NodeTag type;
	IdentNode *target;
	Node *value;
} AssignNode;


IntNode *makeIntNode(int);
FltNode *makeFltNode(float);
IdentNode *makeIdentNode(char*);
ExprNode *makeExprNode(char, Node*, Node*);
AssignNode *makeAssignNode(IdentNode*, Node*);
void displayNode(Node *);
#endif

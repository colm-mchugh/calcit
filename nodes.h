#ifndef _nodes_h_
#define _nodes_h_

#define MAX_TOKEN_LEN 128

typedef enum NodeTag
{
	T_INT_CONST,
	T_FLT_CONST,
	T_IDENT,
	T_EXPR,
	T_ASSIGN,
	T_SYNTAX_ERROR
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
	Node *declaration; 
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
	void *resolved_value;
} AssignNode;

typedef struct syntax_error {
	NodeTag type;
	char *error_message;
} SyntaxErrNode;


IntNode *makeIntNode(const char*);
FltNode *makeFltNode(const char*);
IdentNode *makeIdentNode(const char*);
ExprNode *makeExprNode(char, Node*, Node*);
AssignNode *makeAssignNode(IdentNode*, Node*);
SyntaxErrNode *makeSyntaxErrNode(const char *);

void deleteNode(Node*);
bool genericDeleteNode(void*);
void displayNode(Node *);
void displayIntNode(IntNode*);
void displayFltNode(FltNode*);
void displayIdentNode(IdentNode*);
void displayExprNode(ExprNode*);
void displayAssignNode(AssignNode*);

#endif

#ifndef _context_h_
#define _context_h_
#include "nodes.h"
#include "list.h"
#include "hashtable.h"

// Context is responsible for maintaining state 
	
// An iop is an integer operation:
typedef int (*iop)(int, int);

// A flop is a floating-point operation:
typedef float (*flop)(float, float);

// EvalOps holds all operations (iop and flops)
// operations are indexed by the symbol's character code
// (e.g. the iop for plus ('+') is at index '+')
#define MAX_OPS 128
typedef struct _evalops {
	iop iops[MAX_OPS];
	flop flops[MAX_OPS];
} EvalOps;

// Context structure and create/delete functions:

typedef struct _parsecontext {
	List *parse_tree;
	Hashtable *symbol_table;
	EvalOps *eval_ops;
	List *errors;
} Context;

Context *createContext();

void deleteContext(Context**);

void cleanUpContext(Context*);

// ExprValue is used to hold the value of an expression
typedef struct _expr_value {
	NodeTag type;
	union value_tag {
		int int_value;
		float float_value;
	} value;
} ExprValue;

ExprValue doEval(Context *, ExprValue, ExprValue, char); 

// Error handling functionality

#define MAX_ERROR_MSG_LEN 128

typedef enum ERROR {
	NO_SUCH_IDENT = 0,
	SYNTAX_ERROR = 1
} ErrorTag;

typedef struct _error {
	ErrorTag error_tag;
	char message[MAX_ERROR_MSG_LEN];
} ErrorMessage;

ErrorMessage *makeError(ErrorTag, char*);

#endif

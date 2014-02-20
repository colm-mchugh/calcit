#ifndef _context_h_
#define _context_h_
#include "nodes.h"
#include "list.h"
#include "hashtable.h"

typedef enum ERROR {
	NO_SUCH_IDENT = 0,
	SYNTAX_ERROR = 1
} ErrorTag;

#define MAX_ERROR_MSG_LEN 32

typedef struct _error {
	ErrorTag error_tag;
	char message[MAX_ERROR_MSG_LEN];
} ErrorMessage;
	
typedef struct _expr_value {
	NodeTag type;
	union value_tag {
		int int_value;
		float float_value;
	} value;
} ExprValue;

typedef int (*iop)(int, int);
typedef float (*flop)(float, float);

typedef struct _evalops {
	iop iops[128];
	flop flops[128];
} EvalOps;

typedef struct _parsecontext {
	List *parse_tree;
	Hashtable *symbol_table;
	EvalOps *eval_ops;
	List *errors;
} Context;

Context *createContext();

void deleteContext(Context*);

ErrorMessage *makeError(ErrorTag, char*);

ExprValue doEval(Context *, ExprValue, ExprValue, char); 

void cleanUpContext(Context*);

#endif

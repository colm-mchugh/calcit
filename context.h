#ifndef _context_h_
#define _context_h_
#include "list.h"
#include "hashtable.h"
#include "stack.h"

typedef enum ERROR {
	NO_SUCH_IDENT
} ErrorTag;

typedef struct _error {
	ErrorTag error_tag;
	char *message;
} ErrorMessage;
	
typedef struct _parsecontext {
	List *parse_tree;
	Hashtable *symbol_table;
	Stack *evalStack;
	Stack *errors;
	List *to_delete;
} Context;

Context *createContext();

void deleteContext(Context*);

ErrorMessage *makeError(ErrorTag, char*);

#endif

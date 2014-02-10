#include "context.h"
#include <stdlib.h>
#include <string.h>

Context *createContext() {
	Context *new_context = (Context*)malloc(sizeof(Context));
	new_context->symbol_table = createHash(25);
	new_context->errors = createStack();
	new_context->to_delete = createList();
	return new_context;
}

void deleteContext(Context *ctx) {
	//TODO - free memory allocated 
}

ErrorMessage *makeError(ErrorTag error_tag, char* data) {
	ErrorMessage *em = (ErrorMessage*)malloc(sizeof(ErrorMessage));
	em->error_tag = error_tag;

	switch(error_tag) {
		case NO_SUCH_IDENT: 
			em->message = (char*)malloc(strlen(data)+1);
			strcpy(em->message, data);
			break;
	}
}

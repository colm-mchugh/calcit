#include "stack.h"

// This is retarded, should just be #defines
Stack *createStack() {
	Stack *new_stack = createList();
}

void push(Stack* s, void* data) {
	prependTo(s, data);
}

void *pop(Stack* s) {
	void *data = removeElement(s, 0);
	return data;
}

bool is_empty(Stack *s) {
	return (s->num_nodes == 0);
}


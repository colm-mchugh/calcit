#include "stack.h"


Stack *initStack() {
	Stack *new_stack = createList();
}

void deleteStack(Stack* s) {
	// TODO: free memory allocated by the Stack
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


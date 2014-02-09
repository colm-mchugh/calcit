#ifndef _stack_h
#define _stack_h
#include "list.h"

typedef List Stack;

Stack *initStack();

void deleteStack(Stack*);

void push(Stack*, void*);

void *pop(Stack*);

bool is_empty(Stack*);

#endif

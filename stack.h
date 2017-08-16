#ifndef STACK_H
#define	STACK_H

#include "list.h"

typedef List Stack;

Stack *makeStack();

/**
 * Return 0 if the given stack contains 0 items, 
 * 1 otherwise
 * @param s
 * @return 
 */
bool empty(Stack *s);

/**
 * Remove and return the top item in the given stack.
 * Nop if the stack is empty.
 * @param s
 * @return 
 */
void *pop(Stack *s);

/**
 * Put the given item on top of the stack.
 * @param s
 * @param item
 */
void push(Stack *s, void *item);


#endif	/* STACK_H */


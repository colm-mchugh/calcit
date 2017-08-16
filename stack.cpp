#include "stack.h"
#include <stdlib.h>
#include <assert.h>

Stack *makeStack() {
    return createList();
}

bool empty(Stack *s) {
    assert s != NULL;
    return s->num_nodes == 0;
}

void push(Stack *s, void *item) {
    assert s != NULL;
    prependTo(s, item);
}

void *pop(Stack *s) {
    assert !empty(s);
    return removeElement(s, 0);
}
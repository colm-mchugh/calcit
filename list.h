#ifndef _list_h
#define _list_h

typedef struct _node {
	void *data;
	struct _node *next;
} ListNode;

typedef struct _list {
	ListNode *head;
	ListNode *tail;
} List;

List *createList();

void appendTo(List*, void*);

void prependTo(List*, void*);

#endif

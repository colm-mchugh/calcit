#ifndef _list_h
#define _list_h

typedef struct _node {
	void *data;
	struct _node *next;
	struct _node *prev;
} ListNode;

typedef struct _list {
	ListNode *head;
	ListNode *tail;
	int num_nodes;
} List;

#define list_size(l) (l->num_nodes)

typedef bool (*compare)(void*, void*);

typedef bool (*deleter)(void*);

List *createList();

void deleteList(List**, deleter);

void *getElement(List*, int);

void *getMatch(List *, compare , void *);

void *removeElement(List*, int);

void *removeIfExists(List*, void*);

void appendTo(List*, void*);

void prependTo(List*, void*);

#endif

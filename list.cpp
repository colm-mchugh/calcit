#include "list.h"
#include <stdlib.h>

List *createList() {
	List *new_list = (List*)malloc(sizeof(List));
	new_list->head = NULL;
	new_list->tail = NULL;
	return new_list;
}

ListNode *createListNode(void *data) {
	ListNode *new_node = (ListNode*)malloc(sizeof(ListNode));
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void appendTo(List *list, void *data) {
	ListNode *new_node = createListNode(data);
	if (list->head == NULL) {
		list->tail = new_node;
	}
	new_node->next = list->head;
	list->head = new_node;
}

void prependTo(List *list, void *data) {
	ListNode *new_node = createListNode(data);
	if (list->head == NULL) {
		list->head = new_node;
	} else {
		list->tail->next = new_node;
	}
	list->tail = new_node;
}

#include "list.h"
#include <stdlib.h>

List *createList() {
	List *new_list = (List*)malloc(sizeof(List));
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->num_nodes = 0;
	return new_list;
}

ListNode *createListNode(void *data) {
	ListNode *new_node = (ListNode*)malloc(sizeof(ListNode));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

void deleteList(List** list_ptr, deleter delete_fn ) {
	// free memory allocated for the List
	if ((list_ptr != NULL) && (*list_ptr != NULL)) {
		List *list = *list_ptr;
		ListNode *node = list->head;
		ListNode *next_node = NULL;
		while(node != NULL) {
			// save the next node
			next_node = node->next;
			// hand off node's data to deleter function
			if (delete_fn != NULL) {
				delete_fn(node->data);
			}
			free(node);
			node = next_node;
		}
		free(list);
		*list_ptr = NULL;
	}
}

// Add data to the START of the list
void prependTo(List *list, void *data) {
	ListNode *new_node = createListNode(data);
	if (list->head == NULL) {
		list->tail = new_node;
	} else {
		new_node->next = list->head;
		list->head->prev = new_node;
	}
	list->head = new_node;
	list->num_nodes++;
}

// Add data to the END of the list
void appendTo(List *list, void *data) {
	ListNode *new_node = createListNode(data);
	if (list->head == NULL) {
		list->head = new_node;
	} else {
		list->tail->next = new_node;
		new_node->prev = list->tail;
	}
	list->tail = new_node;
	list->num_nodes++;
}

// get element at index (0-indexed)
void *getElement(List *list, int index) {
	if (index >= list->num_nodes) {
		return NULL;
	}
	int i = 0;
	ListNode *n = list->head;
	while (i++ < index) {
		n = n->next;
	}
	return n->data;
}

// get first element matching comp_op
void *getMatch(List *list, compare comp_op, void *data) {
	int i = 0;
	ListNode *n = list->head;
	while ((n != NULL) && !comp_op(n->data, data) ) {
		n = n->next;
	}
	return (n != NULL? n->data : NULL);
}

void *unlinkElement(List *list, ListNode *element);

void *removeIfExists(List* list, void* data) {
	ListNode *element = list->head;
	while ((element != NULL) && (element->data != data)) {
		element = element->next;
	}
	if (element != NULL) {
		data = unlinkElement(list, element);
	}
	return data;
}

// remove element at index (0-indexed)
void *removeElement(List *list, int index) {
	void *data = NULL;
	if (index >= list->num_nodes) {
		return data;
	}
	ListNode *element = NULL;
	int i = 0;
	element = list->head;
	while (i++ < index) {
		element = element->next;
	}
	data = unlinkElement(list, element);
	return data;
}

void *unlinkElement(List *list, ListNode *element) {
	if (element->prev == NULL) {
		list->head = element->next;
	} else {
		element->prev->next = element->next;
	}
	if (element->next == NULL) {
		list->tail = element->prev;
	} else {
		element->next->prev = element->prev;
	}
	void *data = element->data;
	free(element);
	list->num_nodes--;
	return data;
}
		

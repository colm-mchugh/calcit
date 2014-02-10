#include "hashtable.h"
#include <stdlib.h>

Hashtable *createHash(int num_buckets = 25) {
	Hashtable *new_hash = (Hashtable*)malloc(sizeof(Hashtable));
	new_hash->num_buckets = num_buckets;
	new_hash->buckets = (List**)malloc(sizeof(List**)*num_buckets);
	for(int i = 0; i < new_hash->num_buckets; i++) {
		new_hash->buckets[i] = NULL;
	}
	return new_hash;
}

void deleteHash(Hashtable *hashtable) {
	//TODO :- free the memory associated with hash
}

int hash(Hashkey k) {
	// TODO :- implement this!!
	return 0;
}

void add(Hashtable *h, Hashkey k, void* data) {
	int index = hash(k) % h->num_buckets;
	if (h->buckets[index] == NULL) {
		h->buckets[index] = createList();
	}
	prependTo(h->buckets[index], data);
}

void *lookup(Hashtable *h, Hashkey k, compare mf) {
	int index = hash(k) % h->num_buckets;
	void *data = NULL;
	List *bucket = h->buckets[index];
	if (bucket != NULL) {
		data = getMatch(bucket, mf, k);	
	}
	return data;
}


#include "hashtable.h"
#include <stdlib.h>
#include <string.h>


Hashtable *createHash(int num_buckets = DEFAULT_NUM_BUCKETS) {
	Hashtable *new_hash = (Hashtable*)malloc(sizeof(Hashtable));
	new_hash->num_buckets = num_buckets;
	new_hash->buckets = (List**)malloc(sizeof(List**)*num_buckets);
	for(int i = 0; i < new_hash->num_buckets; i++) {
		new_hash->buckets[i] = NULL;
	}
	return new_hash;
}

void deleteHash(Hashtable **hashtable_ptr, deleter delete_fn) {
	//free the memory associated with hash
	if ((hashtable_ptr != NULL) && (*hashtable_ptr != NULL)) {
		Hashtable *hash_table = *hashtable_ptr;
		for(int i = 0; i < hash_table->num_buckets; i++) {
			deleteList(&hash_table->buckets[i], delete_fn);
		}
		free(hash_table);
		*hashtable_ptr = NULL;
	}
	
}

int hash(Hashkey k, int num_buckets) {
	int hash_val = 0;
	int len = strlen(k);
	for (int i = 0; i < len; i++) {
		hash_val = (DEFAULT_NUM_BUCKETS * hash_val + k[i]) % num_buckets;
	}
	return hash_val;
}

void add(Hashtable *h, Hashkey k, void* data) {
	int index = hash(k, h->num_buckets);
	if (h->buckets[index] == NULL) {
		h->buckets[index] = createList();
	}
	prependTo(h->buckets[index], data);
}

void *lookup(Hashtable *h, Hashkey k, compare mf) {
	int index = hash(k, h->num_buckets);
	void *data = NULL;
	List *bucket = h->buckets[index];
	if (bucket != NULL) {
		data = getMatch(bucket, mf, k);	
	}
	return data;
}


void remove(Hashtable* h, Hashkey k, void* data) {
	int index = hash(k, h->num_buckets);
	List *bucket = h->buckets[index];
	if (bucket != NULL) {
		removeIfExists(bucket, data);
	}
}

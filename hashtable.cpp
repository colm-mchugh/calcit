#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

Hashtable *createHash(int num_buckets = 25) {
	Hashtable *new_hash = (Hashtable*)malloc(sizeof(Hashtable));
	new_hash->num_buckets = num_buckets;
	new_hash->buckets = (List**)malloc(sizeof(List**)*num_buckets);
	for(int i = 0; i < new_hash->num_buckets; i++) {
		new_hash->buckets[i] = NULL;
	}
	return new_hash;
}

void deleteHash(Hashtable **hashtable_ptr) {
	//free the memory associated with hash
	if ((hashtable_ptr != NULL) && (*hashtable_ptr != NULL)) {
		Hashtable *hash_table = *hashtable_ptr;
		for(int i = 0; i < hash_table->num_buckets; i++) {
			deleteList(&hash_table->buckets[i]);
		}
		free(hash_table);
		*hashtable_ptr = NULL;
	}
	
}

int hash(Hashtable *hashtable, Hashkey k) {
	int len = strlen(k);
	return (len + 4 * (k[0] + 4 * k[len/2])) % hashtable->num_buckets;
}

void add(Hashtable *h, Hashkey k, void* data) {
	int index = hash(h, k) % h->num_buckets;
	if (h->buckets[index] == NULL) {
		h->buckets[index] = createList();
	}
	prependTo(h->buckets[index], data);
}

void *lookup(Hashtable *h, Hashkey k, compare mf) {
	int index = hash(h, k) % h->num_buckets;
	void *data = NULL;
	List *bucket = h->buckets[index];
	if (bucket != NULL) {
		data = getMatch(bucket, mf, k);	
	}
	return data;
}


void remove(Hashtable* h, Hashkey k, void* data) {
	int index = hash(h, k) % h->num_buckets;
	List *bucket = h->buckets[index];
	if (bucket != NULL) {
		removeIfExists(bucket, data);
	}
}

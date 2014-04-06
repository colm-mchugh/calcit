#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

// create a hashtable with num_buckets buckets
Hashtable *createHash(int num_buckets = DEFAULT_NUM_BUCKETS) {
	Hashtable *new_hash = (Hashtable*)malloc(sizeof(Hashtable));
	new_hash->num_buckets = num_buckets;
	new_hash->buckets = (List**)malloc(sizeof(List**)*num_buckets);
	for(int i = 0; i < new_hash->num_buckets; i++) {
		new_hash->buckets[i] = NULL; // buckets are created on demand
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
		free(hash_table->buckets);
		free(hash_table);
		*hashtable_ptr = NULL;
	}
	
}

// hash function - return hash of given string key modulo number of buckets.
// (taken from p460 of Algorithms by Sedgewick vol 4)
int hash(Hashkey key, int num_buckets) {
	int hash_val = 0;
	int len = strlen(key);
	for (int i = 0; i < len; i++) {
		hash_val = (DEFAULT_NUM_BUCKETS * hash_val + key[i]) % num_buckets;
	}
	return hash_val;
}

// add key - data to the appropriate bucket
void add(Hashtable *h, Hashkey key, void* data) {
	int index = hash(key, h->num_buckets);
	if (h->buckets[index] == NULL) {
		h->buckets[index] = createList();
	}
	prependTo(h->buckets[index], data);
}

// lookup - determine which bucket; determine if there
// is a value with the given key in that bucket 
void *lookup(Hashtable *h, Hashkey key, compare mf) {
	int index = hash(key, h->num_buckets);
	void *data = NULL;
	List *bucket = h->buckets[index];
	if (bucket != NULL) {
		data = getMatch(bucket, mf, key);	
	}
	return data;
}

// remove - determine which bucket; determine if the value
// we want to remove is in that bucket
void remove(Hashtable* h, Hashkey key, void* data) {
	int index = hash(key, h->num_buckets);
	List *bucket = h->buckets[index];
	if (bucket != NULL) {
		removeIfExists(bucket, data);
	}
}

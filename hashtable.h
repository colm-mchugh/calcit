#ifndef _hashtable_h_
#define _hashtable_h_

#include "list.h"

// Basic hashtable implementation - uses List for 
// each hash bucket

#define DEFAULT_NUM_BUCKETS	31

typedef struct _hashtable {
		List** buckets;
		int num_buckets;
} Hashtable;

typedef char*  Hashkey;

// Allocate a hashtable
Hashtable *createHash(int);

// Delete a hashtable, apply deleter function to elements in the hash
void deleteHash(Hashtable**, deleter);

// Add the given key - data to a hashtable
void add(Hashtable*, Hashkey, void*);

// Return the value for the given hashkey
void *lookup(Hashtable *, Hashkey, compare);

// Remove the given key - value from the hashtable
void remove(Hashtable*, Hashkey, void*);

#endif

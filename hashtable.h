#ifndef _hashtable_h_
#define _hashtable_h_

#include "list.h"

typedef struct _hashtable {
		List** buckets;
		int num_buckets;
} Hashtable;

typedef char*  Hashkey;

Hashtable *createHash(int);

void deleteHash();

void add(Hashtable*, Hashkey, void*);

void *lookup(Hashtable *, Hashkey, compare);

#endif

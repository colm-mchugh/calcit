#ifndef PQUEUE_H
#define	PQUEUE_H

#define INIT_SZ 8

typedef struct _pqueue {
    int *keys;
    int keys_len;
    int N;
} PQueue;

PQueue *newPQueue();

void PQInsert(PQueue *pq, int k);

int PQDelete(PQueue *pq);

int PQPeek(PQueue *pq);

int PQSize(PQueue *pq);

bool PQEmpty(PQueue *pq);

#endif	/* PQUEUE_H */


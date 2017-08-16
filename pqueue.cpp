#include "pqueue.h"
#include <stdlib.h>
#include <assert.h>

#define INIT_SZ 8

PQueue *newPQueue() {
    PQueue *new_pq = (PQueue*) malloc(sizeof (PQueue));
    new_pq->N = 0;
    new_pq->keys_len = INIT_SZ;
    new_pq->keys = (int*) malloc(sizeof (int) * INIT_SZ);
}

void PQInsert(PQueue *pq, int k) {
    if (pq->N > pq->keys_len) {
        pq = PQResize(pq, pq->keys_len * 2);
    }
    pq->keys[pq->N] = k;
    pq->N++;
    PQSwim(pq, pq->N);
}

int PQDelete(PQueue *pq) {
    int k = pq->keys[pq->N];
    PQExch(pq, 1, pq->N--);
    PQSink(pq, 1);
    return k;
}

int PQPeek(PQueue *pq) {
    assert !PQEmpty(pq);
    return pq->keys[0];
}

int PQSize(PQueue *pq) {
    return pq->N;
}

bool PQEmpty(PQueue *pq) {
    return pq->N == 0;
}

void PQSwim(PQueue *pq, int i) {
    while (!PQOrdered(pq, i)) {
        PQExch(pq, i, i / 2);
        i = i / 2;
    }
}

void PQSink(PQueue *pq, int i) {
    while (2 * i <= pq->N && (!PQOrdered(pq, 2 * i) || !PQOrdered(pq, 2 * i + 1))) {
            int j = PQCompare(pq, 2 * i, 2 * i + 1);
            PQExch(pq, i, j);
            i = j;
        }
}

void PQExch(PQueue *pq, int i, int j) {
    int tmp = pq->keys[i - 1];
    pq->keys[i - 1] = pq->keys[j - 1];
    pq->keys[j - 1] = tmp;
}

bool PQOrdered(PQueue *pq, int i) {
    if ((i <= 1) || (i > pq->N)) {
        return true;
    }
    int p = PQParent(pq, i);
    return p >= pq->keys[i];
}

int PQParent(PQueue *pq, int i) {
    return pq->keys[i / 2 - 1];
}

PQueue *PQResize
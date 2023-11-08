//priorityQueue.c
/**
 *
 * @author Max Blennemann
 * @version 11/6/23
 */

#include "stdlib.h"
#include "assert.h"
#include "string.h"
#include "stdbool.h"
#include "pqueue.h"

typedef struct pqueue {
    void** data;
    unsigned count;
    unsigned size;

    int (* compare)();
} PriorityQueue;

#define DEFAULT_SIZE 10

PQ* createQueue(int (* compare)()) {
    PQ* toReturn = malloc(sizeof(PQ));
    assert(toReturn != NULL);
    toReturn->data = malloc(DEFAULT_SIZE * sizeof(void*));
    assert(toReturn->data != NULL);
    toReturn->count = 0;
    toReturn->size = DEFAULT_SIZE;
    toReturn->compare = compare;
    return toReturn;
}

unsigned parent(unsigned i) {
    return (i - 1) / 2;
}

unsigned leftChild(unsigned i) {
    return 2 * i + 1;
}

unsigned rightChild(unsigned i) {
    return 2 * i + 2;
}

void destroyQueue(PQ* pq) {
    assert(pq != NULL);
    unsigned i = 0;
    free(pq->data);
    free(pq);
}

int numEntries(PQ* pq) {
    assert(pq != NULL);
    return pq->count;
}

void downHeapify(PQ* pq) {
    if (pq->count <= 1)
        return;
    unsigned index = 0;
    while (leftChild(index) < pq->count) {
        unsigned left = leftChild(index);
        unsigned right = rightChild(index);
        unsigned min = index;
        if (left < pq->count && pq->compare(pq->data[left], pq->data[min]) > 0)
            min = left;
        if (right < pq->count && pq->compare(pq->data[right], pq->data[min]) > 0)
            min = right;
        if (min != index) {
            void* temp = pq->data[min];
            pq->data[min] = pq->data[index];
            pq->data[index] = temp;
            index = min;
        } else
            break;
    }
}

void upHeapify(PQ* pq) {
    if (pq->count <= 1)
        return;
    unsigned index = pq->count - 1;
    while (index != 0 && pq->compare(pq->data[parent(index)], pq->data[index]) > 0) {
        void* temp = pq->data[parent(index)];
        pq->data[parent(index)] = pq->data[index];
        pq->data[index] = temp;
        index = parent(index);
    }
}

void addEntry(PQ* pq, void* entry) {

    assert(pq != NULL);
    assert(entry != NULL);
    if (pq->count == pq->size) {
        pq->size *= 2;
        pq->data = realloc(pq->data, pq->size * sizeof(void*));
        assert(pq->data != NULL);
    }
    pq->data[pq->count] = entry;
    pq->count++;
    upHeapify(pq);
}

void debugPrint(PQ* pq) {
    unsigned i = 0;
    printf("Debug Below: (printing pq with length %u)\n", pq->count);
    for (; i < pq->count; i++) {
        printf("%i, ", *(int*) pq->data[i]);
    }
}


void* removeEntry(PQ* pq) {
    assert(pq != NULL);
    assert(pq->count > 0);
    void* toReturn = pq->data[0];
    void* temp = pq->data[0];
    pq->data[0] = pq->data[pq->count];
    pq->data[pq->count] = temp;
    pq->count--;
    downHeapify(pq);
    return toReturn;
}
// priorityQueue.c
/**
 * Defines a priority Queue
 * Uses a heap to store data
 *
 * @author Max Blennemann
 * @author 11/13/23
 */
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "pqueue.h"

typedef struct pqueue {
    void** data;
    unsigned count;
    unsigned size;

    int (* compare)(const void*, const void*);
} PQ;

#define DEFAULT_SIZE 10

/**
 * Creates a new priority queue.
 * The compare function should return a negative number if the first argument is less than the second,
 *
 * @param compare the function to compare two elements
 * @return the new priority queue
 * @timeComplexity O(1)
 */
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


/**
 * Returns the parent of the given index.
 *
 * @param i the index
 * @timeComplexity O(1)
 */
unsigned parent(unsigned i) {
    return (i - 1) / 2;
}

/**
 * Returns the right child of the given index.
 *
 * @param i the index
 * @timeComplexity O(1)
 */
unsigned leftChild(unsigned i) {
    return 2 * i + 1;
}

/**
 * Returns the right child of the given index.
 *
 * @param i the index
 * @timeComplexity O(1)
 */
unsigned rightChild(unsigned i) {
    return 2 * i + 2;
}

/**
 * Destroys the queue.
 *
 * @param pq the queue to be destroyed
 * @timeComplexity O(1)
 */
void destroyQueue(PQ* pq) {
    assert(pq != NULL);
    free(pq->data);
    free(pq);
}

/**
 * The number of entries that the queue has.
 *
 * @param pq the queue
 * @return the number of entries
 * @timeComplexity O(1)
 */
int numEntries(PQ* pq) {
    assert(pq != NULL);
    return pq->count;
}

/**
 * Fixes the heap by moving the element at the root downward until the heap property is satisfied.
 *
 * @param pq the priority queue to get the array from to fix
 * @timeComplexity O(log n) where n is the number of elements in the heap
 */
void downHeapify(PQ* pq) {
    if (pq->count <= 1)
        return;
    unsigned index = 0;
    while (leftChild(index) < pq->count) {
        unsigned left = leftChild(index);
        unsigned right = rightChild(index);
        unsigned min = index;
        if (left < pq->count && pq->compare(pq->data[left], pq->data[min]) < 0) {
            min = left;
        }
        if (right < pq->count && pq->compare(pq->data[right], pq->data[min]) < 0) {
            min = right;
        }
        if (min != index) {
            void* temp = pq->data[min];
            pq->data[min] = pq->data[index];
            pq->data[index] = temp;
            index = min;
        } else {
            break;
        }
    }
}

/**
 * Fixes the heap by moving the element at the end upward until the heap property is satisfied.
 *
 * @param pq the priority queue to get the array from to reheapify
 * @timeComplexity O(log(n)) where n is the number of elements in the heap
 */
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

/**
 * Adds an entry to the priority queue.
 *
 * @param pq the queue to add to
 * @param entry the entry
 * @timeComplexity O(1) amortized time; O(n) worst case scenario
 */
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

/**
 * Removes the entry with the highest priority.
 * Returns NULL if the queue is empty.
 * The caller is responsible for freeing the memory.
 *
 * @param pq the priority queue to modify
 * @return the removed item
 * @timeComplexity O(log(n)) where n is the number of elements in the heap
 * because we need to reheapify
 */
void* removeEntry(PQ* pq) {
    assert(pq != NULL);
    if (pq->count == 0) return NULL;
    void* toReturn = pq->data[0];
    void* temp = pq->data[0];
    pq->data[0] = pq->data[pq->count - 1];
    pq->data[pq->count - 1] = temp;
    pq->count--;
    downHeapify(pq);
    return toReturn;
}

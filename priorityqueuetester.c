/*
 * File:	sort.c
 *
 * Copyright:	2017, Darren C. Atkinson
 *
 * Description:	Read in a sequence of integers from the standard input and
 *		write them in sorted order on the standard output.  The
 *		application works by inserting each integer into a priority
 *		queue ADT and then repeatedly removing the smallest value
 *		from the queue and printing it.
 */

# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
#include "priorityQueue.c"

static int intcmp(int* i1, int* i2) {
    assert(i1 != NULL);
    assert(i2 != NULL);
    return (*i1 < *i2) ? -1 : (*i1 > *i2);
}

int main(void) {
    PQ* pq;
    int* p, x;


    pq = createQueue(intcmp);
    while (scanf("%d", &x) == 1) {
        p = malloc(sizeof(int));
        assert(p != NULL);
        *p = x;
        addEntry(pq, p);
    }
    debugPrint(pq);
    printf("debug print successful");

    while (pq->count != 0) {
        debugPrint(pq);
        printf("\n%i; pcount is %u\n", *(int*) removeEntry(pq), pq->count);
    }

    destroyQueue(pq);
    exit(EXIT_SUCCESS);
}

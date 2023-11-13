#include "stdlib.h"
#include "assert.h"
#include "string.h"
#include "stdbool.h"
#include "pack.h"
#include "priorityQueue.c"
#include <stdio.h>
#include <stdlib.h>

#define ASCII_COUNT 256

struct node* makeNode(int data, struct node* left, struct node* right) {
    struct node* newNode = malloc(sizeof(struct node));
    assert(newNode != NULL);
    newNode->parent = NULL;
    newNode->count = data;
    if (left != NULL)
        left->parent = newNode;
    if (right != NULL)
        right->parent = newNode;
    return newNode;
}

static int nodecmp(struct node* i1, struct node* i2) {
    return (i1->count < i2->count) ? -1 : (i1->count > i2->count);
}

void pack(char* infile, char* outfile, struct node* leaves[257]) {
    FILE* in = fopen(infile, "r");
    FILE* out = fopen(outfile, "w");
    assert(in != NULL);
    assert(out != NULL);
    unsigned* frequency = malloc(sizeof(unsigned) * (ASCII_COUNT + 1));//+ 1 for eof
    assert(frequency != NULL);
    //0 the array
    for (int i = 0; i < ASCII_COUNT; i++) {
        frequency[i] = 0;
    }
    int c;
    while ((c = fgetc(in)) != EOF) {
        frequency[c]++;
    }
    for (int i = 0; i < ASCII_COUNT; i++) {
        if (frequency[i] != 0) {
            struct node* newNode = makeNode(frequency[i], NULL, NULL);
            leaves[i] = newNode;
        }
    }
    //create a priority queue to arrange nodes since we only have parents
    PQ* a = createQueue(nodecmp);//TODO check compare function
    for (int i = 0; i < ASCII_COUNT; i++) {
        if (frequency[i] != 0) {
            addEntry(a, leaves[i]);
        }
    }
    //TODO check if this works for eof
    struct node* eofNode = makeNode(1, NULL, NULL);
    leaves[256] = eofNode;
    addEntry(a, eofNode);

    //make priority queue have 1 node
    while (numEntries(a) > 1) {
        struct node* left = removeEntry(a);
        struct node* right = removeEntry(a);
        struct node* newNode = makeNode(left->count + right->count, left, right);
        addEntry(a, newNode);
    }
    struct node* root = removeEntry(a);
    fclose(in);
    fclose(out);
}


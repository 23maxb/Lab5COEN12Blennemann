//huffmanPack.c
/**
 * Compresses string files using huffman pack algorithm.
 *
 * @author Max Blennemann
 * @version 11/13/23
 */
#include "stdlib.h"
#include "assert.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pack.h"
#include "pqueue.h"

#define ASCII_COUNT 256

/**
 * Creates a new node with the given data and children.
 * This function also updates the children's parent.
 * Nodes in this project have no children nodes that are accessible from the parent.
 *
 * @param data
 * @param left
 * @param right
 * @return
 */
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

/**
 * Returns the depth of the node.
 * A node with no parent is defined as a depth of 0.
 *
 * @param a the node to check
 * @return The depth of the node
 * @timeComplexity O(n) where n is the depth of the node
 */
unsigned depth(struct node* a) {
    unsigned depth = 0;
    while (a->parent != NULL) {
        depth++;
        a = a->parent;
    }
    return depth;
}

/**
 * Compares two nodes.
 *
 * @param t1 the first node
 * @param t2 the second node
 * @return -1 if t1's count < t2's count
 *          0 if t1's count == t2's count
 *          1 if t1's count > t2's count
 * @timeComplexity O(1)
 */
static int nodecmp(struct node* t1, struct node* t2) {
    return (t1->count < t2->count) ? -1 : (t1->count > t2->count);
}

/**
 * Packs the given file into the given outfile.
 *
 * @param infile the file to read from
 * @param outfile the file to output to (will be created or overwritten depending if it exists)
 * @timeComplexity O(n) where n is the number of characters in the file
 */
void huffmanPack(char* infile, char* outfile) {
    unsigned i;
    FILE* in = fopen(infile, "r");
    assert(in != NULL);
    unsigned* frequency = malloc(sizeof(unsigned) * (ASCII_COUNT));//+ 1 for eof
    assert(frequency != NULL);

    //0 the array
    for (i = 0; i < ASCII_COUNT; i++) {
        frequency[i] = 0;
    }

    //frequency per letter
    int c;
    while ((c = fgetc(in)) != EOF) {
        frequency[c]++;
    }
    fclose(in);//close file we are done reading

    struct node** leaves = malloc(sizeof(struct node*) * (ASCII_COUNT + 1));

    //If the frequency of a character is not 0 create a new node for it
    //otherwise set it to NULL
    for (i = 0; i < ASCII_COUNT; i++) {
        if (frequency[i] != 0) {
            leaves[i] = makeNode(frequency[i], NULL, NULL);
        } else {
            leaves[i] = NULL;
        }
    }

    //create a priority queue to arrange nodes since we only have parents
    PQ* a = createQueue(nodecmp);
    for (i = 0; i < ASCII_COUNT; i++) {
        if (frequency[i] != 0) {
            addEntry(a, leaves[i]);
        }

    }
    //add 1 more element to the end of leaves for eof
    //TODO check if this works for eof
    struct node* eofNode = makeNode(0, NULL, NULL);
    leaves[ASCII_COUNT] = eofNode;
    addEntry(a, eofNode);

    //make priority queue have 1 element by adding everything together
    while (numEntries(a) > 1) {
        struct node* left = removeEntry(a);
        struct node* right = removeEntry(a);
        struct node* newNode = makeNode(left->count + right->count, left, right);
        addEntry(a, newNode);
    }

    //queue should have 1 element but doesn't matter just destroy the queue
    struct node* root = removeEntry(a);
    printf("\nhello: %d\n", root->count);
    destroyQueue(a);

    //Print out frequency and memory usage of each character
    for (i = 0; i < ASCII_COUNT; i++) {
        if (frequency[i] != 0) {
            if (isprint(i)) {
                printf("%c: %u x %u bits = %u bits\n", i, frequency[i], depth(leaves[i]),
                       frequency[i] * depth(leaves[i]));
            } else {
                printf("%03o: %u x %u bits = %u bits\n", i, frequency[i], depth(leaves[i]),
                       frequency[i] * depth(leaves[i]));
            }
        }
    }
    //run this 1 more time for the eof character
    printf("%03o: %u x %u bits = %u bits\n", i, frequency[i], depth(leaves[i]),
           frequency[i] * depth(leaves[i]));

    //write to file using pack.c
    pack(infile, outfile, leaves);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./huffmanPack infile outfile\n");
        exit(EXIT_FAILURE);
    }
    huffmanPack(argv[1], argv[2]);
    exit(EXIT_SUCCESS);
}

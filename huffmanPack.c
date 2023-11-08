#include "list.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"
#include "stdbool.h"
#include "pack.h"

struct node {
    struct node* parent;
    int count;
};

void pack(char* infile, char* outfile, struct node* leaves[257]) {

}
#ifndef RB_TREE
#define RB_TREE

#include "str.h"
#include "linked_list.h"

typedef struct rbt RBT;

typedef int (*compare_values)(void *, void *);

RBT *rbt_create();
RBT *rbt_insert(RBT *root, String *key, void *value, compare_values cmp_val);
List *rbt_search(RBT *root, String *key);
void rbt_destroy(RBT *root);

#endif

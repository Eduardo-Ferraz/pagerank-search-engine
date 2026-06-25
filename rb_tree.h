#ifndef RB_TREE
#define RB_TREE

#include "linked_list.h"

typedef struct rbt RBT;

typedef void (*act_fnct)(void *);
typedef int (*compare_values)(void *, void *);

RBT *rbt_create();
RBT *rbt_insert(RBT *root, char *key, void *value, compare_values cmp_val);
List *rbt_search(RBT *root, char *key);
void rbt_destroy(RBT *h, act_fnct free_items_fnct, int free_items);

#endif

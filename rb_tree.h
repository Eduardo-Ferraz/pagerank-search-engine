#ifndef RB_TREE
#define RB_TREE

typedef struct rbt RBT;

typedef int (*compare_values)(void *, void *);

RBT *rbt_create();

#endif
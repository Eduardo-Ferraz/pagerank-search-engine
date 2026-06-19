#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "rb_tree.h"
#include "linked_list.h"
#include "str.h"

#define RED true
#define BLACK false

struct rbt {
    String *key;
    List *list_values;

    bool color;
    RBT *left, *right;
};

static RBT* rotate_left(RBT *h);
static RBT* rotate_right(RBT *h);
static void flip_colors(RBT *h);
static bool is_red(RBT *h);

RBT *rbt_create() {
    return NULL;
}

RBT *rbt_insert(RBT *h, String *key, void *value, compare_values cmp_val) {
    if(h == NULL) {
        RBT *rbt = (RBT *) malloc(sizeof(RBT));
        rbt->left = rbt->right = NULL;
        rbt->key = key;
        
        // create a new empty list
        rbt->list_values = list_create();
        list_insert(rbt->list_values, value, cmp_val);

        return rbt;
    }

    int cmp = compare(key, h->key);
    if(cmp > 0) {
        h->right = rbt_insert(h->right, key, value, cmp_val);
    } else if(cmp < 0) {
        h->left = rbt_insert(h->left, key, value, cmp_val);
    } else { // Element in tree
        list_insert(h->list_values, value, cmp_val);
    }

    if(is_red(h->right) && !is_red(h->left)) h = rotate_left(h);
    
    if(is_red(h->left) && is_red(h->left->left)) h = rotate_right(h);

    if(is_red(h->left) && is_red(h->right)) flip_colors(h);

    return h;
}


static RBT* rotate_left(RBT *h) {
    RBT *x = h->right;
    h->right = x->left; 
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    return x;
}

static RBT* rotate_right(RBT *h) {
    RBT *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = RED;
    return x;
}

static void flip_colors(RBT *h) {
    h->color = RED;
    h->left->color = BLACK;
    h->right->color = BLACK;
}

static bool is_red(RBT *h) {
    if(h == NULL) return BLACK;
    return h->color;
}
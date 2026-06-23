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

static RBT *rotate_left(RBT *h);
static RBT *rotate_right(RBT *h);
static void flip_colors(RBT *h);
static bool is_red(RBT *h);
static RBT *insert(RBT *h, String *key, void *value, compare_values cmp_val);

RBT *rbt_create() {
    return NULL;
}

RBT *rbt_insert(RBT *root, String *key, void *value, compare_values cmp_val) {
    if(root == NULL) {
        return insert(root, key, value, cmp_val);
    } else {
        root = insert(root, key, value, cmp_val);
        root->color = BLACK; // a raiz é sempre preta
    }
    return root;
}

static RBT *insert(RBT *h, String *key, void *value, compare_values cmp_val) {
    if(h == NULL) {
        RBT *node = (RBT *) malloc(sizeof(RBT));
        node->left = node->right = NULL;
        node->key = key;
        node->color = RED; // nó novo entra vermelho

        // cria a lista de valores já com o primeiro valor
        node->list_values = list_create();
        // Se o valor é NULL, não insere nada.
        if(value != NULL) list_insert(node->list_values, value, cmp_val);
        

        return node;
    }

    int cmp = compare(key, h->key);
    if(cmp > 0) {
        h->right = insert(h->right, key, value, cmp_val);
    } else if(cmp < 0) {
        h->left = insert(h->left, key, value, cmp_val);
    } else { // chave já está na árvore: adiciona o valor e descarta a chave repetida
        // Se o valor é NULL, não insere nada.
        if(value != NULL) list_insert(h->list_values, value, cmp_val);

        string_destroy(key);
    }

    if(is_red(h->right) && !is_red(h->left)) h = rotate_left(h);

    if(is_red(h->left) && is_red(h->left->left)) h = rotate_right(h);

    if(is_red(h->left) && is_red(h->right)) flip_colors(h);

    return h;
}

List *rbt_search(RBT *h, String *key) {
    while(h != NULL) {
        int cmp = compare(key, h->key);
        if(cmp < 0) {
            h = h->left;
        } else if(cmp > 0) {
            h = h->right;
        } else {
            return h->list_values;
        }
    }

    return NULL; // não achou
}

void rbt_destroy(RBT *h, act_fnct free_items_fnct, int free_items) {
    if(h == NULL) return;

    rbt_destroy(h->left, free_items_fnct, free_items);
    rbt_destroy(h->right, free_items_fnct, free_items);

    string_destroy(h->key);
    list_destroy(h->list_values, free_items_fnct, free_items); // libera a lista, e, opcionalmente, os valores guardados
    free(h);
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

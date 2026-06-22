#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

typedef struct node Node;
struct node {
    void *item;
    Node *next;
};

struct listIterator {
    Node *current;
};

struct list {
    Node *first;
    Node *last;
};

List *list_create() {
    List *l = (List *) malloc(sizeof(List));
    l->first = l->last = NULL;
    return l;
}

ListIterator *iterator_create(List *l) {
    ListIterator *it = (ListIterator *) malloc(sizeof(ListIterator));
    it->current = l->first;
    return it;
}

void *get_and_advance(ListIterator *it) {
    void *item;
    if(it->current) {
        item = it->current->item;
        it->current = it->current->next;
    } else {
        item = NULL;
    }

    return item;
}

void list_insert(List *l, void *item, compare_items cmp_item) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->item = item;
    n->next = NULL;

    // case 1 - empty list
    if(l->first == NULL) {
        l->first = l->last = n;
        return;
    }

    // PT-BR:
    // Para o nosso caso, se um elemento se repetir, ele vai acontecer sucessivamente.
    // Então, vou verificar se o último valor é igual ao que vai ser inserido. Se for, não
    // insere.
    if(cmp_item(item, l->last->item) == 0) {
        free(n);
        return;
    }

    // else, insert on the end
    l->last->next = n;
    l->last = n;
}

void traverse_list(List *l, act_fnct act) {
    Node *n = l->first;
    while(n != NULL) {
        act(n->item);
        n = n->next;
    }
}

void iterator_destroy(ListIterator *it) {
    free(it);
}

void list_destroy(List *l) {
    Node *n = l->first;
    while(n != NULL) {
        Node *next = n->next;
        free(n); // libera o nó, não o n->item (que é de outro dono)
        n = next;
    }
    free(l);
}
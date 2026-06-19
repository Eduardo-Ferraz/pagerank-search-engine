#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct list List;
typedef struct listIterator ListIterator;

typedef int (*compare_items)(void *, void *);
typedef void (*act_fnct)(void *);

List *list_create();
ListIterator *iterator_create(List *l);
void *get_and_advance(ListIterator *it);
void list_insert(List *l, void *item, compare_items cmp_item);
void traverse_list(List *l, act_fnct act);

#endif
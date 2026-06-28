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
void list_append(List *l, void *item);
int get_list_length(List *l);
void traverse_list(List *l, act_fnct act);
void iterator_destroy(ListIterator *it);
void copy_list_to_vector(List *l, void **vector);
int intersect_list_with_vector(List *l, void **vector, int vector_size, compare_items compare_fnct);
void list_destroy(List *l, act_fnct free_fnct, int free_items);

#endif
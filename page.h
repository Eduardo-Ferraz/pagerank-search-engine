#ifndef PAGE_H
#define PAGE_H

#include "str.h"
#include "linked_list.h"

typedef struct page Page;

Page *page_create(String *page_name);
void page_destroy(void *page);
int page_compare(void *p1, void *p2);
String *get_page_name(Page *p);
void page_print(void *page);

void page_set_id(Page *p, int id);
int page_get_id(Page *p);

void page_set_rank(Page *p, double rank);
double page_get_rank(Page *p);

void page_add_link(Page *from, Page *to);
int page_get_out_degree(Page *p);
List *page_get_links(Page *p);

#endif

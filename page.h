#ifndef PAGE_H
#define PAGE_H

#include "str.h"

typedef struct page Page;

Page *page_create(String *page_name);
int page_compare(void *p1, void *p2);
String *get_page_name(Page *p);
void page_print(void *page);

#endif
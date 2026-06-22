#include <stdlib.h>
#include "page.h"

struct page {
    String *page_name;
    double page_rank;
};

Page *page_create(String *page_name) {
    Page *page = (Page *) malloc(sizeof(Page));
    page->page_name = page_name;
    page->page_rank = -1;

    return page;
}

/**
 * Compares two page by name.
 * 
 * The function returns:
 * < 0, if p1 < p2,
 * > 0, if p1 > p2,
 * = 0, if p1 = p2.
 *  */ 
int page_compare(void *p1, void *p2) {
    Page *page_1 = (Page *) p1;
    Page *page_2 = (Page *) p2;
    return compare(page_1->page_name, page_2->page_name);
}

String *get_page_name(Page *p) {
    return p->page_name;
}

void page_print(void *page) {
    Page *p = (Page *) page;
    string_print_from(p->page_name, 0);
}
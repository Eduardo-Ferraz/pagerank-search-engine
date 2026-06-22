#include <stdlib.h>
#include "page.h"

struct page {
    int id;            // índice da página, 0..n-1 (o "i" do enunciado)
    String *page_name;
    double page_rank;
    List *out_links;   // pra quais páginas essa aqui aponta (guarda Page*)
    int out_degree;    // quantos links de saída ela tem
};

Page *page_create(String *page_name) {
    Page *page = (Page *) malloc(sizeof(Page));
    page->id = -1;
    page->page_name = page_name;
    page->page_rank = -1;
    page->out_links = list_create();
    page->out_degree = 0;

    return page;
}

// compara duas páginas pelo nome.
// retorna < 0 se p1 < p2, > 0 se p1 > p2, e 0 se forem iguais.
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

void page_set_id(Page *p, int id) {
    p->id = id;
}

int page_get_id(Page *p) {
    return p->id;
}

void page_set_rank(Page *p, double rank) {
    p->page_rank = rank;
}

double page_get_rank(Page *p) {
    return p->page_rank;
}

// liga a página "from" na "to": "from" ganha um link de saída pra "to"
void page_add_link(Page *from, Page *to) {
    list_append(from->out_links, to);
    from->out_degree++;
}

int page_get_out_degree(Page *p) {
    return p->out_degree;
}

// devolve a lista de páginas pra onde "p" aponta (Page*).
// a lista é da própria página, não pode liberar.
List *page_get_links(Page *p) {
    return p->out_links;
}

// libera a página: o nome, a lista de links (só a lista, não as páginas
// apontadas, que são de outro dono) e ela mesma.
void page_destroy(void *page) {
    Page *p = (Page *) page;
    string_destroy(p->page_name);
    list_destroy(p->out_links);
    free(p);
}

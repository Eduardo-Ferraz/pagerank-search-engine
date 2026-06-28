#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "page.h"

struct page
{
    int id; // índice da página, 0..n-1 (o "i" do enunciado)
    char *page_name;
    double page_rank;
    List *out_links; // pra quais páginas essa aqui aponta (guarda Page*)
    int out_degree;  // quantos links de saída ela tem
};

// cria a página fazendo a sua própria cópia do nome
Page *page_create(char *page_name)
{

    Page *page = (Page *)malloc(sizeof(Page));

    page->id = -1;
    page->page_name = strdup(page_name);
    page->page_rank = -1;
    page->out_links = list_create();
    page->out_degree = 0;

    return page;
}

// compara duas páginas pelo nome (sem diferenciar maiúscula de minúscula).
// retorna < 0 se p1 < p2, > 0 se p1 > p2, e 0 se forem iguais.
int page_name_compare(void *p1, void *p2)
{
    Page *page_1 = (Page *)p1;
    Page *page_2 = (Page *)p2;
    return strcasecmp(page_1->page_name, page_2->page_name);
}

// compara duas páginas pelo id
// retorna < 0 se p1 < p2, > 0 se p1 > p2, e 0 se forem iguais.
int page_id_compare(void *p1, void *p2) 
{
    Page *page_1 = (Page *)p1;
    Page *page_2 = (Page *)p2;
    // printf("comparando %s:%d com %s:%d\n", page_1->page_name, page_1->id, page_2->page_name, page_2->id);
    return page_1->id - page_2->id;
}

int page_rank_compare_qosrt(const void *p1, const void *p2)
{
    Page *page_1 = *(Page **)p1;
    Page *page_2 = *(Page **)p2;

    double diff = page_1->page_rank - page_2->page_rank;
    
    if(diff > 0) {
        return -1;
    } else if(diff < 0) {
        return 1;
    } else {
        return strcasecmp(page_1->page_name, page_2->page_name);
    }
}

char *get_page_name(Page *p)
{
    return p->page_name;
}

void page_print(void *page)
{
    Page *p = (Page *)page;
    fputs(p->page_name, stdout);
    printf(" ");
}

void page_set_id(Page *p, int id)
{
    p->id = id;
}

int page_get_id(Page *p)
{
    return p->id;
}

void page_set_rank(Page *p, double rank)
{
    p->page_rank = rank;
}

double page_get_rank(Page *p)
{
    return p->page_rank;
}

// adiciona um link de saída: a página de origem passa a apontar pra de destino
void page_add_link(Page *from, Page *to)
{
    list_append(from->out_links, to);
    from->out_degree++;
}

int page_get_out_degree(Page *p)
{
    return p->out_degree;
}

// devolve a lista de páginas pra onde "p" aponta (Page*).
// a lista é da própria página, não pode liberar.
List *page_get_links(Page *p)
{
    return p->out_links;
}

// libera a página: o nome (cópia feita na criação), a lista de links (só a lista,
// não as páginas apontadas, que são de outro dono) e ela mesma.
void page_destroy(void *page)
{
    Page *p = (Page *)page;
    free(p->page_name);

    list_destroy(p->out_links, page_destroy, 0);

    free(p);
}

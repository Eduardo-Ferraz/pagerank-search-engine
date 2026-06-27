#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "rb_tree.h"
#include "indexer.h"
#include "linked_list.h"
#include "graph.c"
#include "pagerank.h"


RBT* mount_pages_tree(char *source_dir, char *buffer, Page ***pages_out, int *n_out)
{
    RBT *pages_tree = rbt_create();

    // caminho do index.txt
    sprintf(buffer, "%s%s", source_dir, "index.txt");
    FILE *f_index = fopen(buffer, "r");
    if (!f_index)
    {
        perror("Erro ao ler index.txt");
        exit(1);
    }

    int capacity = 16;
    int count = 0;
    Page **pages = (Page **) malloc(capacity * sizeof(Page *)); // cria um vetor de páginas para facilitar o cálculo do pagerank futuramente

    Page *page;
    while ((fgets(buffer, 256, f_index) != NULL))
    {
        // tira o \n do fim
        buffer[strcspn(buffer, "\n")] = '\0';
        page = page_create(buffer); // page_create faz a própria cópia do nome
        page_set_id(page, count); // seta id das páginas
        pages_tree = rbt_insert(pages_tree, buffer, page, page_compare);

        if (count == capacity) // checa se o limite do vetor vai estourar e faz realloc
        {
            capacity *= 2;
            Page **tmp = (Page **) realloc(pages, capacity * sizeof(Page *));
            if (!tmp)
            {
                perror("Erro de alocacao do array de paginas");
                exit(1);
            }
            pages = tmp;
        }

        pages[count] = page;
        count++;
    }

    fclose(f_index);

    *pages_out = pages;
    *n_out = count;
    return pages_tree;
}

RBT *mount_stopwords_tree(char* source_dir, char *buffer)
{
    RBT *stopwords_tree = rbt_create();

    sprintf(buffer, "%s%s", source_dir, "stopwords.txt");
    FILE *f_sw = fopen(buffer, "r");
    if (!f_sw)
    {
        perror("Erro ao ler stopwords.txt");
        exit(1);
    }

    while ((fgets(buffer, 256, f_sw) != NULL))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        // passa o buffer direto: a árvore copia a palavra (chave)
        stopwords_tree = rbt_insert(stopwords_tree, buffer, NULL, NULL);
    }

    // Fechando arquivo de leitura
    fclose(f_sw);

    return stopwords_tree;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Erro: informe o diretório com os arquivos de entrada");
        exit(1);
    }

    char *source_dir = argv[1];
    char buffer[256];

    /**
    Lê o index.txt e cria uma struct Page para cada
    página listada no index.txt
    Também, conta quantas páginas foram lidas
    */
    Page **pages;
    int n = 0;
    RBT* pages_tree = mount_pages_tree(source_dir, buffer, &pages, &n);

    /**
    Lê todas as stop words e armazena na árvore rubro negra (não terão valores associados,
    servirá apenas para consulta.
    */
    RBT *stopwords_tree = mount_stopwords_tree(source_dir, buffer);

    /**
    Lê cada palavra das páginas. Se for stopword, ignora.
    Caso contrário, adiciona à tabela de símbolos principal
    */
    RBT *symbol_table = mount_symbol_table(pages_tree, stopwords_tree, source_dir, buffer);

    /**
    Monta o grafo. Cada página passa a apontar para seus links.
    */
    mount_graph(pages_tree, source_dir, buffer);

    /**
    Calcula os valores de pagerank.
    */
    pagerank_compute(pages, n);

    // Teste de consulta à tabela de símbolos (passa a palavra direto, sem alocar)
    List *l = rbt_search(symbol_table, "abacate");
    traverse_list(l, page_print);
    printf("\n");

    free(pages);
    rbt_destroy(symbol_table, NULL, 0);
    rbt_destroy(stopwords_tree, NULL, 0);
    rbt_destroy(pages_tree, page_destroy, 1);
}

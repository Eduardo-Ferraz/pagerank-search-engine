#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "rb_tree.h"
#include "indexer.h"
#include "linked_list.h"
#include "graph.h"
#include "pagerank.h"

List *get_smaller_list(char *search, RBT *symbol_table, RBT *stopwords_tree);
int mount_intersection_vector(char *search, void **vector, int vector_size, RBT *symbol_table, RBT *stopwords_tree);
void print_search_result(char *search, void **intersection_vector, int sz);

RBT *mount_pages_tree(char *source_dir, char *buffer, int *n_out)
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

    Page *page;
    while ((fgets(buffer, 256, f_index) != NULL))
    {
        // tira o \n do fim
        buffer[strcspn(buffer, "\n")] = '\0';
        page = page_create(buffer); // page_create faz a própria cópia do nome
        page_set_id(page, count);   // seta id das páginas
        pages_tree = rbt_insert(pages_tree, buffer, page, page_name_compare);

        count++;
    }

    fclose(f_index);

    *n_out = count;
    return pages_tree;
}

RBT *mount_stopwords_tree(char *source_dir, char *buffer)
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
    Também, conta quantas páginas foram lidas e cria um vetor de páginas para auxiliar no cálculo do pagerank
    */
    int n_pages = 0;
    RBT *pages_tree = mount_pages_tree(source_dir, buffer, &n_pages);

    /**
    Lê todas as stop words e armazena na árvore rubro negra (não terão valores associados,
    servirá apenas para consulta.
    */
    RBT *stopwords_tree = mount_stopwords_tree(source_dir, buffer);

    /**
    Lê cada palavra das páginas. Se for stopword, ignora.
    Caso contrário, adiciona à tabela de símbolos principal

    Aproveita essa função para montar uma lista de páginas ordenada lexicograficamente.
    */
    Page **pages = (Page **)malloc(sizeof(Page *) * n_pages);
    RBT *symbol_table = mount_symbol_table(pages_tree, stopwords_tree, source_dir, buffer, pages);

    /**
    Monta o grafo. Cada página passa a apontar para seus links.
    */
    mount_graph(pages_tree, source_dir, buffer);

    /**
    Calcula os valores de pagerank.
    */
    pagerank_compute(pages, n_pages);

    // Search
    while (fgets(buffer, 256, stdin) != NULL)
    {
        // tira o \n do fim
        buffer[strcspn(buffer, "\n")] = '\0';

        // Aqui, faz a cópia da menor lista da palavras da consulta para um vetor.
        List *smaller_list = get_smaller_list(buffer, symbol_table, stopwords_tree);
        int intersection_max_size = get_list_length(smaller_list);
        int intersection_size = 0;

        if (intersection_max_size != 0)
        {
            void *intersection_vector[intersection_max_size];

            copy_list_to_vector(smaller_list, intersection_vector);

            intersection_size = mount_intersection_vector(buffer, intersection_vector, intersection_max_size, symbol_table, stopwords_tree);

            print_search_result(buffer, intersection_vector, intersection_size);
        }
        else
        {
            print_search_result(buffer, NULL, 0);
        }
    }

    free(pages);
    rbt_destroy(symbol_table, NULL, 0);
    rbt_destroy(stopwords_tree, NULL, 0);
    rbt_destroy(pages_tree, page_destroy, 1);
}

// Função que modifica o vector original, deixando apenas a interseção das listas.
// Retorna a quantidade de intersecções encontradas.
int mount_intersection_vector(char *search, void **vector, int vector_size, RBT *symbol_table, RBT *stopwords_tree)
{
    char *search_copy = strdup(search);

    List *list = NULL;
    char *word = strtok(search_copy, " ");
    int sz = vector_size;

    while (word != NULL)
    {
        if (!rbt_search(stopwords_tree, word))
        {
            if ((list = rbt_search(symbol_table, word)) != NULL)
            {
                sz = intersect_list_with_vector(list, vector, sz, page_id_compare);
            }
        }

        word = strtok(NULL, " ");
    }

    free(search_copy);

    return sz;
}

List *get_smaller_list(char *search, RBT *symbol_table, RBT *stopwords_tree)
{
    char *search_copy = strdup(search);

    List *smaller_list = NULL;
    List *list = NULL;
    char *word = strtok(search_copy, " ");

    while (word != NULL)
    {
        if (!rbt_search(stopwords_tree, word))
        {
            if ((list = rbt_search(symbol_table, word)) != NULL)
            {
                if (smaller_list == NULL)
                    smaller_list = list;

                if (get_list_length(list) < get_list_length(smaller_list))
                    smaller_list = list;
            }
        }

        word = strtok(NULL, " ");
    }

    free(search_copy);

    return smaller_list;
}

void print_search_result(char *search, void **intersection_vector, int sz)
{
    if (intersection_vector != NULL)
    {
        qsort(intersection_vector, sz, sizeof(void *), page_rank_compare_qosrt);
    }

    printf("search:%s\n", search);

    printf("pages:");
    for (int i = 0; i < sz; i++)
    {
        // espaço entre páginas, não no fim da linha
        if (i > 0)
        {
            printf(" ");
        }
        printf("%s", get_page_name(intersection_vector[i]));
    }
    printf("\n");

    printf("pr:");
    for (int i = 0; i < sz; i++)
    {
        if (i > 0)
        {
            printf(" ");
        }
        printf("%f", page_get_rank(intersection_vector[i]));
    }
    printf("\n");
}

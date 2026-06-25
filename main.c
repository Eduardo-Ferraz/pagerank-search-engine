#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "rb_tree.h"
#include "linked_list.h"

void mount_page_path(char *source_dir, Page *p, char *buffer)
{
    sprintf(buffer, "%spages/%s", source_dir, get_page_name(p));
}

void read_all_index(FILE *f_index, List *list, char *buffer)
{
    Page *page;
    while ((fgets(buffer, 256, f_index) != NULL))
    {
        // tira o \n do fim
        buffer[strcspn(buffer, "\n")] = '\0';
        page = page_create(buffer); // page_create faz a própria cópia do nome
        list_insert(list, page, page_compare);
    }
}

RBT *read_all_stopwords(FILE *f_sw, char *buffer)
{
    RBT *stopwords_tree = rbt_create();

    while ((fgets(buffer, 256, f_sw) != NULL))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        // passa o buffer direto: a árvore copia a palavra (chave)
        stopwords_tree = rbt_insert(stopwords_tree, buffer, NULL, NULL);
    }

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

    // Lendo o index.txt (primeira parte do programa).
    List *page_list = list_create();

    // caminho do index.txt
    sprintf(buffer, "%s%s", source_dir, "index.txt");
    FILE *f_index = fopen(buffer, "r");
    if (!f_index)
    {
        perror("Erro ao ler index.txt");
        exit(1);
    }

    /**
    Lê o index.txt e cria uma struct Page para cada
    página listada no index.txt
    */
    read_all_index(f_index, page_list, buffer);
    fclose(f_index);

    /**
    Lê todas as stop words e armazena na árvore rubro negra (não terão valores associados,
    servirá apenas para consulta.
    */
    sprintf(buffer, "%s%s", source_dir, "stopwords.txt");
    FILE *f_sw = fopen(buffer, "r");
    if (!f_sw)
    {
        perror("Erro ao ler stopwords.txt");
        exit(1);
    }
    RBT *stopwords_tree = read_all_stopwords(f_sw, buffer);
    fclose(f_sw);

    /**
    Lê cada palavra das páginas. Se for stopword, ignora.
    Caso contrário, adiciona à tabela de símbolos principal
    */
    RBT *symbol_table = rbt_create();

    /**
    Aqui, recupera um iterador e, para cada página, lê o conteúdo dela e monta
    a árvore rubro negra.
    */
    ListIterator *it = iterator_create(page_list);
    Page *p;
    FILE *f_page;
    while ((p = (Page *)get_and_advance(it)) != NULL)
    {
        mount_page_path(source_dir, p, buffer);

        f_page = fopen(buffer, "r");

        // Lê palavra por palavra. Passa o próprio buffer direto pra árvore: não
        // aloca nada aqui, a árvore só copia a palavra quando o termo é novo.
        while (fscanf(f_page, " %255[a-zA-Z0-9-]", buffer) == 1)
        {
            if (!rbt_search(stopwords_tree, buffer))
            {
                symbol_table = rbt_insert(symbol_table, buffer, p, page_compare);
            }
        }

        fclose(f_page);
    }

    // Teste de consulta à tabela de símbolos (passa a palavra direto, sem alocar)
    List *l = rbt_search(symbol_table, "melhor");
    traverse_list(l, page_print);
    printf("\n");

    iterator_destroy(it);
    list_destroy(page_list, page_destroy, 1);
    rbt_destroy(symbol_table, NULL, 0);
    rbt_destroy(stopwords_tree, NULL, 0);
}

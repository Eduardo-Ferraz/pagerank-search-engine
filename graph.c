#include "graph.h"
#include "page.h"
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

static Page *find_page(RBT *pages_tree, char *name)
{
    List *l = rbt_search(pages_tree, name);
    if (l == NULL)
        return NULL;
    ListIterator *it = iterator_create(l);
    Page *p = (Page *)get_and_advance(it);
    iterator_destroy(it);
    return p;
}

void mount_graph(RBT *pages_tree, char *source_dir, char *buffer)
{
    sprintf(buffer, "%s%s", source_dir, "graph.txt");
    FILE *f_graph = fopen(buffer, "r");
    if (f_graph == NULL)
    {
        perror("Erro ao ler graph.txt.");
        exit(1);
    }

    char origin_name[256];
    char dest_name[256];
    int out_degree;

    while (fscanf(f_graph, " %255s %d", origin_name, &out_degree) == 2)
    {
        Page *origin = find_page(pages_tree, origin_name);
        if(origin == NULL)
        {
            perror("Erro: pagina de origem nao existe no index.");
            exit(1);
        }

        for(int i = 0; i < out_degree; i++)
        {
            if(fscanf(f_graph, " %255s", dest_name) != 1)
            {
                perror("Erro: pagina de destino faltante; grafo mal-formado.");
                exit(1);
            }

            Page *dest = find_page(pages_tree, dest_name);
            if(dest == NULL)
            {
                perror("Erro: pagina de destino nao existe no indice.");
                exit(1);
            }

            page_add_link(origin, dest);
        }
    }
    fclose(f_graph);
}
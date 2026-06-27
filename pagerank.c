#include "pagerank.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "linked_list.h"

/**
 * reseta o valor do pr_new de todas as páginas
 */
static void reset_pr_new(double *pr_new, int n)
{
    for (int i = 0; i < n; i++)
    {
        pr_new[i] = (1.0 - ALPHA) / n;
    }
}

/**
 * distribui a contribuição de cada página para quem ela aponta
 */
static void distribute(Page **pages, double *pr_old, double *pr_new, int n)
{
    for (int i = 0; i < n; i++)
    {
        int out_degree = page_get_out_degree(pages[i]);

        if (out_degree == 0)
        {
            // caso especial: página sem saída se retroalimenta
            pr_new[i] += ALPHA * pr_old[i];
        }
        else
        {
            List *links = page_get_links(pages[i]);
            ListIterator *it = iterator_create(links);
            Page *dest;
            while ((dest = (Page *)get_and_advance(it)) != NULL)
            {
                int k = page_get_id(dest);
                pr_new[k] += ALPHA * pr_old[i] / out_degree;
            }
            iterator_destroy(it);
        }
    }
}

/**
 * Calcula E(k)
 */
static double diff(double *pr_new, double *pr_old, int n)
{
    double E = 0.0;
    for (int i = 0; i < n; i++)
    {
        E += fabs(pr_new[i] - pr_old[i]);
    }
    return E /= n;
}

void pagerank_compute(Page **pages, int n)
{
    // vetores representando os valores de pagerank para cada uma das páginas
    double *pr_old = (double *)malloc(n * sizeof(double));
    double *pr_new = (double *)malloc(n * sizeof(double));

    // inicialização: PR(0)(i) = 1/n para todo i
    for (int i = 0; i < n; i++)
    {
        pr_old[i] = 1.0 / n;
    }

    double E; // média das diferenças absolutas entre duas iterações sucessivas
    do
    {
        reset_pr_new(pr_new, n);
        distribute(pages, pr_old, pr_new, n);
        E = diff(pr_new, pr_old, n);

        // pr_new da iteração atual vira o pr_old da próxima iteração
        double *tmp = pr_old;
        pr_old = pr_new;
        pr_new = tmp;
    } while (E >= EPSILON);

    // grava o resultado final (está em pr_old, por causa da última troca)
    for (int i = 0; i < n; i++)
    {
        page_set_rank(pages[i], pr_old[i]);
    }

    free(pr_old);
    free(pr_new);
}
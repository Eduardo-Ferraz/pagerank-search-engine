#ifndef PAGERANK_H
#define PAGERANK_H

#include "page.h"

#define ALPHA 0.85 // valor especificado no trabalho para a fórmula de pagerank
#define EPSILON 1e-6 // valor de parada especificado no trabalho apra a fórmula ed pagerank

/**
 * Calcula o PageRank de cada página via Power Method, e grava
 * o resultado em cada Page via page_set_rank.
 *
 * 
 *  - pages[0..n-1] já tem id atribuído
 *  - mount_graph já foi chamado
 */
void pagerank_compute(Page **pages, int n);

#endif
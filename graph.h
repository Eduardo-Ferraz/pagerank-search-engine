#ifndef GRAPH_H
#define GRAPH_H

#include "rb_tree.h"

/**
 * Lê o arquivo graph.txt dentro de source_dir e, para cada linha,
 * conecta as páginas correspondentes em pages_tree via page_add_link.
 *
 * pages_tree já deve estar populada (mount_pages_tree
 * já deve ter sido chamado antes).
 *
 * buffer auxiliar reutilizado para montar o caminho do arquivo
 * (mesmo padrão usado em mount_pages_tree / mount_stopwords_tree).
 */
void mount_graph(RBT *pages_tree, char *source_dir, char *buffer);

#endif
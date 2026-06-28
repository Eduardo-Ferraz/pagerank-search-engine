#ifndef INDEXER_H
#define INDEXER_H

#include "rb_tree.h"
#include "page.h"

RBT *mount_symbol_table(RBT *pages_tree, RBT *stopwords_tree, char* source_dir, char *buffer, Page **pages);

#endif
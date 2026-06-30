#include <stdio.h>
#include <stdlib.h>
#include "indexer.h"
#include "page.h"
#include "linked_list.h"

typedef struct
{
    RBT *stopwords_tree;
    RBT *indexer_tree;
    char *source_dir;
    char *buffer;

    // Ponteiro para montar a lista de páginas (usada no pagerank)
    // (como já se sabe a quantidade de nós) da árvore, não preciso passar internamente).
    Page **pages;

    // contador de páginas já visitadas (id de cada pag)
    int count;
} IndexerContext;

static void visit_page_node(char *key, List *value, void *indexer_context);

static void mount_page_path(char *source_dir, char *page_name, char *buffer)
{
    sprintf(buffer, "%spages/%s", source_dir, page_name);
}

/**
 * Explicação:
 *
 * Dado que eu tenho uma árvore de páginas (pós leitura do index.txt), eu preciso:
 *
 * 1. Percorrer cada nó da árvore individualmente;
 * 2. Para cada página, eu monto o path completo até o arquivo.txt dela;
 * 3. Leio esse arquivo.txt e insiro as palavras na árvore do Indexador;
 *
 * Observação: A struct de contexto é para poder deixar a função de percorrer a árvore genérica.
 * Todas as informações que a função de callback precise usar (que não seja unicamente as informações do nó -
 * que são as únicas informações que o TAD rb_tree.h tem acesso) devem ser inseridas nessas struct de contexto.
 *
 *
 */
RBT *mount_symbol_table(RBT *pages_tree, RBT *stopwords_tree, char *source_dir, char *buffer, Page **pages)
{
    // Criando contexto
    IndexerContext *indexer_context = (IndexerContext *)malloc(sizeof(IndexerContext));
    indexer_context->indexer_tree = rbt_create();
    indexer_context->stopwords_tree = stopwords_tree;
    indexer_context->source_dir = source_dir;
    indexer_context->buffer = buffer;

    // Para montar a lista de páginas (usada no pagerank)
    indexer_context->pages = pages;
    indexer_context->count = 0;

    // Visitando cada nó recursivamente
    rbt_traverse(pages_tree, visit_page_node, indexer_context);

    // Recuperando a árvore
    RBT *indexer_tree = indexer_context->indexer_tree;

    free(indexer_context);

    return indexer_tree;
}

// Função de visita do indexador
static void visit_page_node(char *key, List *value, void *indexer_context)
{
    // Desempacota contexto
    IndexerContext *context = (IndexerContext *)indexer_context;
    char *buffer = context->buffer;

    // Retorno o ponteiro para a página e cria, no buffer, o path até ela.
    ListIterator *it = iterator_create(value);
    Page *p = (Page *)get_and_advance(it);
    iterator_destroy(it);
    mount_page_path(context->source_dir, get_page_name(p), buffer);

    // id menor para a menor página.
    // Insensível à ordem que index.txt forneceu (árvore visita na ordem alfabética ).
    context->pages[context->count] = p;
    page_set_id(p, context->count);
    context->count++;

    /**
     * Agora, basta abrir o arquivo e ler as palavras, adicionando-as à árvore do indexador.
     *
     * Obs.: Apenas se a palavra não for uma stopword.
     * */
    FILE *f_page = fopen(buffer, "r");
    if (!f_page)
    {
        perror("Erro ao ler os arquivos de páginas.");
        exit(1);
    }

    // Lê palavra por palavra. Passa o próprio buffer direto pra árvore: não
    // aloca nada aqui, a árvore só copia a palavra quando o termo é novo.
    while (fscanf(f_page, " %255[a-zA-Z0-9-]", buffer) == 1)
    {
        if (!rbt_search(context->stopwords_tree, buffer))
        {
            context->indexer_tree = rbt_insert(context->indexer_tree, buffer, p, page_name_compare);
        }
    }

    fclose(f_page);
}

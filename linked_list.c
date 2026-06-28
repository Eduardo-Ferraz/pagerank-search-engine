#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

typedef struct node Node;
struct node
{
    void *item;
    Node *next;
};

struct listIterator
{
    Node *current;
};

struct list
{
    int num_itens;
    Node *first;
    Node *last;
};

List *list_create()
{
    List *l = (List *)malloc(sizeof(List));
    
    l->num_itens = 0;
    l->first = l->last = NULL;
    return l;
}

ListIterator *iterator_create(List *l)
{
    ListIterator *it = (ListIterator *)malloc(sizeof(ListIterator));

    it->current = l->first;

    return it;
}

void *get_and_advance(ListIterator *it)
{
    void *item;
    if (it->current)
    {
        item = it->current->item;

        it->current = it->current->next;
    }
    else
    {
        item = NULL;
    }

    return item;
}

void list_insert(List *l, void *item, compare_items cmp_item)
{
    Node *n = (Node *)malloc(sizeof(Node));

    n->item = item;

    n->next = NULL;

    // caso 1 - lista vazia
    if (l->first == NULL)
    {
        l->first = l->last = n;
        l->num_itens++;
        return;
    }

    // Para o nosso caso, se um elemento se repetir, ele vai acontecer sucessivamente.
    // Então, vou verificar se o último valor é igual ao que vai ser inserido. Se for, não
    // insere.
    if (cmp_item(item, l->last->item) == 0)
    {
        free(n);
        return;
    }

    // senão, insere no fim
    l->last->next = n;
    l->last = n;
    l->num_itens++;
}

void traverse_list(List *l, act_fnct act)
{
    if (l == NULL)
    {
        return; // consulta sem resultado: nada a percorrer
    }
    Node *n = l->first;
    while (n != NULL)
    {
        act(n->item);
        n = n->next;
    }
}

int get_list_length(List *l) {
    return l != NULL ? l->num_itens : 0;
}

void iterator_destroy(ListIterator *it)
{
    free(it);
}

void list_destroy(List *l, act_fnct free_fnct, int free_items)
{
    Node *n = l->first;

    while (n != NULL)
    {
        Node *next = n->next;
        if (free_items)
        {
            free_fnct(n->item);
        }
        free(n); // libera o nó, e, opcionalmente, o item guardado
        n = next;
    }
    free(l);
}

// adiciona o item no fim da lista, sem checar repetição
void list_append(List *l, void *item)
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->item = item;
    n->next = NULL;

    if (l->first == NULL)
    {
        l->first = l->last = n;
    }
    else
    {
        l->last->next = n;
        l->last = n;
    }

    l->num_itens++;
}

void copy_list_to_vector(List *l, void **vector) {
    int i = 0;
    
    if (l == NULL)
    {
        return; // consulta sem resultado: nada a percorrer
    }
    Node *n = l->first;
    while (n != NULL)
    {
        vector[i] = n->item;
        i++;
        n = n->next;
    }
}

/**
 * Faz a interseção da lista com o vetor.
 * 
 * Observação: Esta função pressupõe que tanto o vector, quanto, a lista, estão ordenados.
 * Portanto, se aproveita disso para fazer a intersecção.
 * 
 * Guarda o resultado dentro do vetor. 
 * 
 * Retorna o tamanho "válido" do vetor.
 */
int intersect_list_with_vector(List *l, void **vector, int vector_size, compare_items compare_fnct) {
    int n = 0; // Número de intersecções encontradas
    int k = 0; // Posição para guardar os valores da intersecção
    int i = 0; // índice para caminhar no vetor
    Node *node = l->first;

    while((i < vector_size) && (node != NULL)) {
        int cmp = compare_fnct(vector[i], node->item);
        // Nesse caso, o elemento do vetor é menor que o elemento da lista.
        // Portanto, não há intersecção, preciso avançar a leitura no vetor para tentar "igualar à lista".

        // Obs.: Note que, portanto, não incremento K (enquanto incremento i). Isto significa, (na primeira iteração, pelo menos)
        // Que a posição 0 do vetor pode ser sobrescrita, pois não há intersecção neste elemento.

        // O pensamento se repete para as iterações posteriores.
        if(cmp < 0) {
            i++;
        }
        // Nesse caso, o elemento do vetor é maior que o elemento da lista.
        // Portanto, não há intersecção, preciso avançar a leitura da lista para tentar "igualar" ao vetor.
        else if(cmp > 0) {
            node = node->next;
        }

        // Nesse caso, houve intersecção, eu faço o shift do vector na primeira posição válida para escrita.
        // E avanço a posição da escrita. Assim como o ponteiro da lista, e o índice do vector.

        // Também incrementa a quantidade de intersecções encontradas.
        else {
            vector[k] = vector[i];
            n++;
            k++;

            i++;
            node = node->next;
        }
    }


    return n;
}
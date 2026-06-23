#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "str.h"
#include "rb_tree.h"
#include "linked_list.h"

void read_all_index(FILE *f_index, List *list, char *buffer);
void mount_page_path(char *source_dir, Page *p, char *buffer);
RBT* read_all_stopwords(FILE *f_sw, char *buffer);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Error: Enter the directory containing the input files");
        exit(1);
    }

    char *source_dir = argv[1];
    char buffer[256];

    // Lendo o index.txt (primeira parte do programa).
    List *page_list = list_create();

    // index path
    sprintf(buffer, "%s%s", source_dir, "index.txt");
    FILE *f_index = fopen(buffer, "r");
    if(!f_index) {
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
    if(!f_sw) {
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
    Aqui, recupera um iterador e, para cada página, lê o contéudo dela e monta
    a árvore rubro negra. 
    */
    ListIterator *it = iterator_create(page_list);
    Page *p;
    FILE *f_page;
    String *key;
    while((p = (Page *) get_and_advance(it)) != NULL) {
        /**
        Esta função coloca no buffer o path de cada página (que será lida e armazenada na árvore).
        Observação: o path, portanto, é uma string comum do C. Que termina em \0.
        */
        mount_page_path(source_dir, p, buffer);

        // Abrindo arquivo;
        f_page = fopen(buffer, "r");
        
        // Lê cada palavra. Se não está na lista de stopwords, adiciona à tabela de símbolos.
        while(fscanf(f_page, " %255[a-zA-Z0-9-]", buffer) == 1) {
            key = string_create(buffer, strlen(buffer));
            if(!rbt_search(stopwords_tree, key)) {
                symbol_table = rbt_insert(symbol_table, key, p, page_compare);
            } else {
                string_destroy(key);
            }
        }

        fclose(f_page);
        
    }

    // Teste de consulta à tabela de símbolos
    String *consulta = string_create("melhor", 6);
    List *l = rbt_search(symbol_table, consulta);

    traverse_list(l, page_print);
    printf("\n");
    string_destroy(consulta);

    iterator_destroy(it);
    list_destroy(page_list, page_destroy, 1);
    rbt_destroy(symbol_table, NULL, 0);
    rbt_destroy(stopwords_tree, NULL, 0);
    

    //traverse_list(page_list, page_print);
}

void mount_page_path(char *source_dir, Page *p, char *buffer) {
    char dir[10] = "pages/";

    // Adding source dir (without \0)
    int str_size = strlen(source_dir);
    memcpy(buffer, source_dir, strlen(source_dir));

    // Adding "pages/" (without \0)
    memcpy(buffer+str_size, dir, strlen(dir));
    str_size += strlen(dir);

    // Adding page name (without \0)
    String *page_name = get_page_name(p);
    char *page_name_content = get_string_content(page_name);
    int page_name_len = get_string_len(page_name);
    memcpy(buffer+str_size, page_name_content, sizeof(char)*page_name_len);

    str_size += page_name_len;

    // Adding \0
    buffer[str_size] = '\0';
}

void read_all_index(FILE *f_index, List *list, char *buffer) {
    String *page_name;
    Page *page;
    while((fgets(buffer, 256, f_index) != NULL)) {
        // Read until \n (includes \n).
        buffer[strcspn(buffer, "\n")] = '\0';
        page_name = string_create(buffer, strlen(buffer));
        page = page_create(page_name);
        list_insert(list, page, page_compare);
    }
}

RBT* read_all_stopwords(FILE *f_sw, char *buffer) {
    RBT *stopwords_tree = rbt_create();
    String *stopword;

    while((fgets(buffer, 256, f_sw) != NULL)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        stopword = string_create(buffer, strlen(buffer));
        stopwords_tree = rbt_insert(stopwords_tree, stopword, NULL, NULL);
    }

    return stopwords_tree;
}
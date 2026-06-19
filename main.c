#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "str.h"
#include "rb_tree.h"
#include "linked_list.h"

void read_all_index(FILE *f_index, List *list, char *buffer);
void mount_page_path(char *source_dir, Page *p, char *buffer);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Error: Enter the directory containing the input files");
        exit(1);
    }

    char *source_dir = argv[1];
    char buffer[256];

    // Reading index
    List *page_list = list_create();
    // index path
    sprintf(buffer, "%s%s", source_dir, "index.txt");
    FILE *f_index = fopen(buffer, "r");
    read_all_index(f_index, page_list, buffer);

    // PT-BR:
    // Agora, recupera um iterador e, para cada página, lê o contéudo dela e monta
    // a árvore rubro negra.

    ListIterator *it = iterator_create(page_list);
    Page *p;
    while((p = (Page *) get_and_advance(it)) != NULL) {
        // page path
        mount_page_path(source_dir, p, buffer);

        printf("%s\n", buffer);
        
    }

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
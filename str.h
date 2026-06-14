#ifndef STR_H
#define STR_H

typedef struct string String;

String *string_create(char *c, int len);
void string_print_from(String *string, int d);
int compare_from(String *s1, String *s2, int d);
int get_string_len(String *string);
char char_at(String *string, int d);
void string_destroy(String *string);

#endif
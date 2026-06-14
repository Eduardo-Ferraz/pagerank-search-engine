#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./str.h"

struct string {
    char *c;
    int len;
};

/**
 * Allocates and initializes a new string (the attribute len shouldn't include the '\0' terminator).
 */
String *string_create(char *c, int len) {
    String *s = (String *) malloc(sizeof(String));
    s->c = (char *) malloc(sizeof(char)*len);
    memcpy(s->c, c, sizeof(char)*len);
    s->len = len;

    return s;
}

/**
 * Prints the string to stdout, starting from index d to the end.
 */
void string_print_from(String *string, int d) {
    fwrite((string->c+d), sizeof(char), (string->len - d), stdout);
}

/**
 * Compares two strings, starting from index d.
 * 
 * The function returns:
 * < 0, if s1 < s2,
 * > 0, if s1 > s2,
 * = 0, if s1 = s2.
 *  */ 
int compare_from(String *s1, String *s2, int d) {
    int min = s1->len < s2->len ? s1->len : s2->len;

    for(int i = d; i < min; i++) {
        if(s1->c[i] < s2->c[i]) return -1;
        if(s1->c[i] > s2->c[i]) return 1;
    }

    // If the prefix is the same
    return s1->len - s2->len;
}

/** Returns the string length. */ 
int get_string_len(String *string) {
    return string->len;
}

/**
 * Returns the character at index "d" of the string.
 * If "d" is out of range, returns -1.
*/
char char_at(String *string, int d) {
    return d < string->len ? string->c[d] : -1;
}

/**
 * Frees the memory allocated for the string.
 */
void string_destroy(String *string) {
    if(string == NULL) return;
    free(string->c);
    free(string);
}


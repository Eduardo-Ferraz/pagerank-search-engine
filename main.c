#include <stdio.h>
#include <stdlib.h>
#include "str.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Error: Enter the directory containing the input files");
        exit(1);
    }

    char *source_dir = argv[1];

}
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define bufSize 1024    // assuming no line can be over 1024 characters long

char *docfile;
int K;

int main(int argc, char *argv[]) {
    if (argc < 5 || strcmp(argv[1], "-i") || strcmp(argv[3], "-k") || !isdigit(*argv[4])) {
        fprintf(stderr, "Invalid arguments: Please run \"$ ./minisearch -i docfile -k K\"\n");
        return 1;
    }
    docfile = argv[2];
    K = atoi(argv[4]);
    if (K < 1) {
        fprintf(stderr, "K must be greater than 0.\n");
        return 1;
    }
    int lines_count = 0;
    printf("%s\n", docfile);
    FILE *fp = fopen(docfile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open %s.\n", docfile);
        return 2;
    }
    char *buffer = malloc(bufSize);
    for (int i = 0; ; i++) {
        if (fgets(buffer, bufSize, fp) == NULL) {
            break;
        }
        if (atoi(buffer) != i) {
            fprintf(stderr, "Error in %s - Docs not in order.\n", docfile);
            return 3;
        }
        printf("%s\n", buffer);
        lines_count++;
    }
//    while ((ch = getc(fp)) != EOF) {
//        if (ch == '\n') {
//            lines_count++;
//        }
//    }
    printf("Lines: %d\n", lines_count);
    return 0;
}



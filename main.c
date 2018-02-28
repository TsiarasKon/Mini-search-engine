#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define bufSize 1024    // assuming no line can be over 1024 characters long

char *docfile;
int K;

int main(int argc, char *argv[]) {
    /// different ways?
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
    int doc_count = 0;
    printf("%s\n", docfile);
    FILE *fp = fopen(docfile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open %s.\n", docfile);
        return 2;
    }
    char *buffer = malloc(bufSize);
    char *bufferptr = buffer;       // used to free buffer in the end, since we're using strtok
    for (int i = 0; ; i++) {
        if (fgets(buffer, bufSize, fp) == NULL) {
            break;
        }
        if (atoi(buffer) != i) {
            fprintf(stderr, "Error in %s - Docs not in order.\n", docfile);
            return 3;
        }
        printf("%d\n", strlen(buffer));
        doc_count++;
    }
    rewind(fp);     // start again from the beginning of docfile
    char **docs = malloc(doc_count * sizeof(char*));
    for (int i = 0; i < doc_count; i++) {
        if (fgets(buffer, bufSize, fp) == NULL) {
            fprintf(stderr, "Something unexpected happened.\n");
            return -1;
        }
        buffer = strchr(buffer, ' ') + 1;     // remove index number in front of doc
        strtok(buffer, "\n");     // remove trailing newline character
        docs[i] = malloc(strlen(buffer));
        strcpy(docs[i], buffer);
    }

    char *command;
    char *cmds[4];
    cmds[0] = "/search";
    cmds[1] = "/df";
    cmds[2] = "/tf";
    cmds[3] = "/exit";
    while (1) {
        printf("Type a command:\n");
        fgets(buffer, bufSize, stdin);
        strtok(buffer, "\n");     // remove trailing newline character
        command = strtok(buffer, " ");
        if (!strcmp(command, cmds[0])) {              // search

        } else if (!strcmp(command, cmds[1])) {       // df

        } else if (!strcmp(command, cmds[2])) {       // tf
            command = strtok(buffer, " ");
        } else if (!strcmp(command, cmds[3])) {       // exit
            break;
        } else {
            printf("Unkown command '%s' - Type a valid command:\n", command);
            /// Implement /help
        }
    }
    for (int i = 0; i < doc_count; i++) {
        free(docs[i]);
    }
    free(docs);
    free(bufferptr);
    return 0;
}



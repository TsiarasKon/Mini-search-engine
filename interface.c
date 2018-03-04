#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

#define bufSize 1024    ///

void interface(Trie *trie, int K) {
    char *command;
    char *cmds[4];
    cmds[0] = "/search";
    cmds[1] = "/df";
    cmds[2] = "/tf";
    cmds[3] = "/exit";
    char *buffer = malloc(bufSize);
    char *bufferptr = buffer;       // used to free buffer in the end, since we're using strtok
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
            printf("Unknown command '%s' - Type a valid command:\n", command);
            /// Implement /help
        }
    }
    free(bufferptr);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

void interface(Trie *trie, int K) {
    char *command;
    char *cmds[4];
    cmds[0] = "/search";
    cmds[1] = "/df";
    cmds[2] = "/tf";
    cmds[3] = "/exit";
    size_t bufsize = 32;      // sample size - getline will reallocate memory as needed
    char *buffer = malloc(bufsize);
    char *bufferptr = buffer;       // used to free buffer in the end, since we're using strtok
    while (1) {
        printf("Type a command:\n");
        getline(&buffer, &bufsize, stdin);
        strtok(buffer, "\r\n");     // remove trailing newline character
        command = strtok(buffer, " ");
        if (!strcmp(command, cmds[0])) {              // search

        } else if (!strcmp(command, cmds[1])) {       // df
            command = strtok(NULL, " \t");
            if (command == NULL) {          // full df
                printTrie(trie);
            } else {        // single word df
                PostingList *postingList = getPostingList(trie, command);
                if (postingList == NULL) {
                    printf("%s doesn't exist in any of the docs.\n", command);
                    continue;
                }
                printf("%s %d\n", command, postingList->df);
            }
        } else if (!strcmp(command, cmds[2])) {       // tf
            command = strtok(NULL, " \t");
            if (command == NULL || !isdigit(*command)) {
                printf("Invalid use of '/tf'.\n");
                continue;
            }
            int id = atoi(command);
            command = strtok(NULL, " \t");
            if (command == NULL) {
                printf("Invalid use of '/tf'.\n");
                continue;
            }
            PostingList *postingList = getPostingList(trie, command);
            if (postingList == NULL) {
                printf("%s doesn't exist in any of the docs.\n", command);
                continue;
            }
            printf("%d %s %d\n", id, command, getTermFrequency(postingList, id));
        } else if (!strcmp(command, cmds[3])) {       // exit
            break;
        } else {
            printf("Unknown command '%s'.\n", command);
            /// Implement /help
        }
    }
    free(bufferptr);
}


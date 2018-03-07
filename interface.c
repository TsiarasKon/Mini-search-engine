#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
#include "pairingheap.h"
#include "util.h"

void interface(Trie *trie, int K, char **docs, int doc_count) {
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
            command = strtok(NULL, " \t");
            if (command == NULL) {
                printf("Invalid use of '/search' - At least one query term is required.\n");
                continue;
            }
            char *terms[10];
            terms[0] = command;
            for (int i = 1; i < 10; i++) {
                terms[i] = NULL;
            }
            int term_count = 1;
            command = strtok(NULL, " \t");
            while (command != NULL && term_count < 10) {
                terms[term_count] = command;
                term_count++;
                command = strtok(NULL, " \t");
            }

            HeapNode *heap = NULL;
            /// calculate score

//            heap = heapInsert(heap, 0.12345678, 7);
//            heap = heapInsert(heap, 0.52345, 589);
//            heap = heapInsert(heap, 3.246, 3564);
//            heap = heapInsert(heap, 0.96486888888, 9766);
//            heap = heapInsert(heap, 0.04326943, 234);
            print_results(heap, K, docs, doc_count);
            destroyHeap(heap);
        } else if (!strcmp(command, cmds[1])) {       // df
            command = strtok(NULL, " \t");
            if (command == NULL) {          // full df
                printTrie(trie);
            } else {        // single word df
                PostingList *postingList = getPostingList(trie, command);
                if (postingList == NULL) {
                    printf("\"%s\" doesn't exist in any of the docs.\n", command);
                    continue;
                }
                printf("%s %d\n", command, postingList->df);
            }
        } else if (!strcmp(command, cmds[2])) {       // tf
            command = strtok(NULL, " \t");
            if (command == NULL || !isdigit(*command)) {
                printf("Invalid use of '/tf' - No doc specified.\n");
                continue;
            }
            int id = atoi(command);
            command = strtok(NULL, " \t");
            if (command == NULL) {
                printf("Invalid use of '/tf' - No word specified.\n");
                continue;
            }
            PostingList *postingList = getPostingList(trie, command);
            // If the word doesn't exist in the asked doc, its termFrequency will be 0
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


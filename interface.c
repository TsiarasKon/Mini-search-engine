#define _GNU_SOURCE         // for getline()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
#include "pairingheap.h"
#include "util.h"

extern int K;
extern int doc_count;

void interface(Trie *trie, char **docs, int *docWc) {
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
        if (!strcmp(command, cmds[0]) || !strcmp(command, "/s")) {              // search
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
            ListNode *postingListPtr[term_count];
            PostingList *tempPostingList;
            for (int i = 0; i < term_count; i++) {
                tempPostingList = getPostingList(trie, terms[i]);
                postingListPtr[i] = (tempPostingList == NULL) ? NULL : tempPostingList->first;
            }
            double doc_score;
            int tf;
            for (int id = 0; id < doc_count; id++) {
                doc_score = 0;
                for (int i = 0; i < term_count; i++) {
                    while (postingListPtr[i] != NULL && postingListPtr[i]->id_times[0] < id) {
                        postingListPtr[i] = postingListPtr[i]->next;
                    }
                    if (postingListPtr[i] == NULL || postingListPtr[i]->id_times[0] > id) {
                        continue;
                    }
                    // Else doc_id exists in this posting list:
                    tempPostingList = getPostingList(trie, terms[i]);
                    tf = getTermFrequency(tempPostingList, id);
                    if (tf <= 0) {   // getPostingList() returned NULL <=> word doesn't exist in trie
                        continue;   // score would be 0 anyway
                    }
                    doc_score += score(tf, tempPostingList->df, docWc[id]);
                }
                if (doc_score != 0) {
                    heap = heapInsert(heap, doc_score, id);
                }
            }
            print_results(&heap, docs);
            if (heap != NULL) {
                destroyHeap(&heap);
            }
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


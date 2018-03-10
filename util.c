#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "util.h"

#define k1 1.2
#define b 0.75

extern int K;
extern int doc_count;
extern double avgdl;

double IDF(int df) {
    return log10((doc_count - df + 0.5) / (df + 0.5));
}

double score(int tf, int df, int D) {
    return IDF(df) * ((tf * (k1 + 1)) / (tf + k1 * (1 - b + b * (D / avgdl))));
}

char word_in(char *word, char **word_list) {
    int i = 0;
    while (word_list[i] != NULL) {
        if (strcmp(word, word_list[i]) == 0) {
            return 1;
        }
        i++;
    }
    return 0;
}

void print_results(HeapNode **heap, char **docs, char **terms) {
    if (*heap == NULL) {
        printf("No results found.\n");
    }
    int margins[4];
    margins[1] = ((int) log10(K)) + 1;
    margins[2] = ((int) log10(doc_count)) + 1;
    margins[3] = 4;         // score decimal precision
    margins[0] = margins[1] + margins[2] + margins[3] + 9;    // total margin sum including parenthesis, braces, etc
    for (int k = 0; k < K; k++) {
        if (*heap == NULL) {
            break;
        }
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if (w.ws_col <= margins[0] + 1) {       // terminal too narrow - cannot pretty print
            printf("%*d.(%*d)[%*.*f] %s\n", margins[1], k, margins[2], (*heap)->id, margins[3] + 3, margins[3], (*heap)->score, docs[(*heap)->id]);
            *heap = deleteMaxNode(heap);
        } else {
            int curr_col;
            size_t curr_word_len;
            int cols_to_write = w.ws_col - margins[0];
            char underlines[cols_to_write];
            char result = 0;    // 0 for the first time, 1 for the rest, -1 for '^'
            char *curr_doc = malloc(strlen(docs[(*heap)->id]) + 1);
            char *curr_doc_ptr = curr_doc;
            strcpy(curr_doc, docs[(*heap)->id]);
            char *curr_word = strtok(curr_doc, " \t");
            while (curr_word != NULL || result <= 0) {
                if (result >= 0) {
                    if (result == 0) {
                        printf("%*d.(%*d)[%*.*f] ", margins[1], k, margins[2], (*heap)->id, margins[3] + 3, margins[3], (*heap)->score);
                    } else {
                        for (int i = 0; i < margins[0]; i++) {
                            printf(" ");
                        }
                    }
                    result = -1;
                    for (int i = 0; i < cols_to_write; i++) {
                        underlines[i] = ' ';
                    }
                    curr_col = 0;
                    while (curr_col < cols_to_write - 1 && curr_word != NULL) {
                        curr_word_len = strlen(curr_word);
                        if (curr_word_len > cols_to_write - curr_col - 1) {     // word doesn't fit in terminal
                            break;
                        }
                        if (word_in(curr_word, terms) == 1) {
                            for (int i = curr_col; i < curr_col + curr_word_len; i++) {
                                underlines[i] = '^';
                            }
                        }
                        for (int i = 0; i < curr_word_len; i++) {
                            printf("%c", curr_word[i]);
                            curr_col++;
                        }
                        printf(" ");
                        curr_col++;
                        curr_word = strtok(NULL, " \t");
                    }
                    printf("\n");
                } else {
                    for (int i = 0; i < margins[0]; i++) {
                        printf(" ");
                    }
                    for (curr_col = 0; curr_col < cols_to_write; curr_col++) {
                        printf("%c", underlines[curr_col]);
                    }
                    result = 1;
                }
            }
            free(curr_doc_ptr);
            *heap = deleteMaxNode(heap);
        }
    }
}

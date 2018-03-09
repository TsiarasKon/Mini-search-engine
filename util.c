#include <stdio.h>
#include <math.h>
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

void print_results(HeapNode **heap, char **docs) {
    if (*heap == NULL) {
        printf("No results found.\n");
    }
    int margins[3];
    margins[0] = ((int) log10(K)) + 1;
    margins[1] = ((int) log10(doc_count)) + 1;
    margins[2] = 4;
    for (int i = 0; i < K; i++) {
        if (*heap == NULL) {
            break;
        }
        printf("%*d.(%*d)[%*f] %s\n", margins[0], i, margins[1], (*heap)->id, margins[2], (*heap)->score, docs[(*heap)->id]);
        *heap = deleteMaxNode(heap);
    }
}

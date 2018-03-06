#include <stdio.h>
#include <math.h>
#include "util.h"

#define k1 1.2
#define b 0.75

double IDF(Trie *root, char *word, int N) {
    PostingList *postingList = getPostingList(root, word);
    if (postingList == NULL) {
        return -1;
    }
    return 5;//log10((N - postingList->df + 0.5) / (postingList->df + 0.5));
}

double score(Trie *root, char *word, int id, int D, double avgdl, int N) {
    int tf = getTermFrequency(getPostingList(root, word), id);
    if (tf < 0) {
        return -1;
    }
    return IDF(root, word, N) * ((tf * (k1 + 1)) / (tf + k1 * (1 - b + b * (D / avgdl))));
}

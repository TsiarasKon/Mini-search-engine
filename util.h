#ifndef UTIL_H
#define UTIL_H

#include "trie.h"
#include "pairingheap.h"

double IDF(Trie *root, char *word, int N);
double score(Trie *root, char *word, int id, int D, double avgdl, int N);

void print_results(HeapNode *heap, int K, char **docs, int doc_count);

#endif

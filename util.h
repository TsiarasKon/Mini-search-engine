#ifndef UTIL_H
#define UTIL_H

#include "trie.h"
#include "pairingheap.h"

double IDF(int df);
double score(int tf, int df, int D);

void print_results(HeapNode **heap, char **docs);

#endif

#ifndef UTIL_H
#define UTIL_H

#include "trie.h"

double IDF(Trie *root, char *word, int N);
double score(Trie *root, char *word, int id, int D, double avgdl, int N);

#endif

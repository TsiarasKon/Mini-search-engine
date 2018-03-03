#ifndef TRIE_H
#define TRIE_H

#include "postinglist.h"

typedef struct postinglist PostingList;     // necessary forward declaration

typedef struct trienode TrieNode;
typedef struct trie Trie;

struct trienode {
    char value;
    TrieNode *next;
    TrieNode *child;
    PostingList *postingList;
};

struct trie {
    TrieNode *first;
};

TrieNode* createTrieNode(char value);
Trie* createTrie();

void directInsert(TrieNode *current, char *word, int id, int i);
void insert(Trie *root, char *word, int id);

void printTrieNode(TrieNode *node, char *prefix);
void printTrie(Trie *root);

#endif

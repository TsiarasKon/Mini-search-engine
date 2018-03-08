#ifndef TRIE_H
#define TRIE_H

#include "postinglist.h"

#ifndef TYPEDEFS     // needed forward declarations, avoiding redeclarations
#define TYPEDEFS
typedef struct postinglist PostingList;
typedef struct trienode TrieNode;
#endif
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

TrieNode* createTrieNode(char value, TrieNode *next);
void deleteTrieNode(TrieNode **trieNode);
Trie* createTrie();
void deleteTrie(Trie **trie);

void directInsert(TrieNode *current, char *word, int id, int i);
void insert(Trie *root, char *word, int id);

PostingList *getPostingList(Trie *root, char *word);

void printTrieNode(TrieNode *node, char *prefix);
void printTrie(Trie *root);

#endif

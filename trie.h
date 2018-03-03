#ifndef TRIE_H
#define TRIE_H

typedef struct listnode {
    int id_times[2];
    struct listnode *next;
} ListNode;

typedef struct postinglist {
    int df;
    ListNode *first;
} PostingList;

typedef struct trienode {
    char value;
    struct trienode *next;
    struct trienode *child;
    PostingList *postingList;
} TrieNode;

typedef struct {
    TrieNode *first;
} Trie;

ListNode* createListNode(int id);
Trie* createTrie();
TrieNode* createTrieNode(char value);
PostingList* createPostingList();
void incrementPostingList(TrieNode *node, int id);

void directInsert(TrieNode *current, char *word, int id, int i);
void insert(Trie *root, char *word, int id);

void printNode(TrieNode *node, char *prefix);
void printTrie(Trie *root);

#endif

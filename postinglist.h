#ifndef POSTINGLIST_H
#define POSTINGLIST_H

#include "trie.h"

typedef struct trienode TrieNode;      // necessary forward declaration

typedef struct listnode ListNode;
typedef struct postinglist PostingList;

struct listnode {
    int id_times[2];
    ListNode *next;
};

struct postinglist {
    int df;
    ListNode *first;
};

ListNode* createListNode(int id);
PostingList* createPostingList();

void incrementPostingList(TrieNode *node, int id);

#endif

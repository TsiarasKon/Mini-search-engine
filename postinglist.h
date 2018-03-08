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
    ListNode *last;
};

ListNode* createListNode(int id);
void deleteListNode(ListNode **listNode);
PostingList* createPostingList();
void deletePostingList(PostingList **postingList);

void incrementPostingList(TrieNode *node, int id);
int getTermFrequency(PostingList *postingList, int id);

#endif

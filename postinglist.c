#include <stdlib.h>
#include "postinglist.h"
#include "trie.h"

ListNode* createListNode(int id) {
    ListNode *listNode = malloc(sizeof(ListNode));
    listNode->id_times[0] = id;
    listNode->id_times[1] = 1;
    listNode->next = NULL;
    return listNode;
}

PostingList* createPostingList() {
    PostingList *postingList = malloc(sizeof(PostingList));
    postingList->df = 0;
    postingList->first = NULL;
    return postingList;
}

/// free all

void incrementPostingList(TrieNode *node, int id) {
    ListNode **current = &(node->postingList->first);
    while (*current != NULL) {
        if ((*current)->id_times[0] == id) {
            (*current)->id_times[1]++;
            return;
        }
        *current = (*current)->next;
    }
    *current = createListNode(id);
    node->postingList->df++;
}
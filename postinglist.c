#include <stdio.h>
#include <stdlib.h>
#include "postinglist.h"

ListNode* createListNode(int id) {
    ListNode *listNode = malloc(sizeof(ListNode));
    listNode->id_times[0] = id;
    listNode->id_times[1] = 1;
    listNode->next = NULL;
    return listNode;
}

void deleteListNode(ListNode *listNode) {
    if (listNode == NULL) {
        fprintf(stderr, "Attempted to delete a NULL ListNode.\n");
        return;
    }
    if (listNode->next != NULL) {
        deleteListNode(listNode->next);
    }
    free(listNode);
}

PostingList* createPostingList() {
    PostingList *postingList = malloc(sizeof(PostingList));
    postingList->df = 0;
    postingList->first = NULL;
    return postingList;
}

void deletePostingList(PostingList *postingList) {
    if (postingList == NULL) {
        fprintf(stderr, "Attempted to delete a NULL PostingList.\n");
        return;
    }
    if (postingList->first != NULL) {
        deleteListNode(postingList->first);
    }
    free(postingList);
}

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

int getTermFrequency(PostingList *postingList, int id) {
    if (postingList == NULL) {
        return 0;
    }
    ListNode *current = postingList->first;
    while (current != NULL) {
        if (current->id_times[0] == id) {
            return current->id_times[1];
        }
        current = current->next;
    }
    return 0;
}

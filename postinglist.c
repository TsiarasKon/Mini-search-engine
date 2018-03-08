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

void deleteListNode(ListNode **listNode) {
    if (*listNode == NULL) {
        fprintf(stderr, "Attempted to delete a NULL ListNode.\n");
        return;
    }
    ListNode *current = *listNode;
    ListNode *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *listNode = NULL;
}

PostingList* createPostingList() {
    PostingList *postingList = malloc(sizeof(PostingList));
    postingList->df = 0;
    postingList->first = postingList->last = NULL;
    return postingList;
}

void deletePostingList(PostingList **postingList) {
    if (*postingList == NULL) {
        fprintf(stderr, "Attempted to delete a NULL PostingList.\n");
        return;
    }
    if ((*postingList)->first != NULL) {
        deleteListNode(&(*postingList)->first);
    }
    free(*postingList);
    *postingList = NULL;
}

void incrementPostingList(TrieNode *node, int id) {
    PostingList **PostingList = &node->postingList;
    if ((*PostingList)->first == NULL) {
        (*PostingList)->first = createListNode(id);
        (*PostingList)->last = (*PostingList)->first;
        return;
    }
    if ((*PostingList)->last->id_times[0] == id) {   // word belongs to the same (last) doc
        (*PostingList)->last->id_times[1]++;
    } else {
        (*PostingList)->last->next = createListNode(id);
        (*PostingList)->last = (*PostingList)->last->next;
        (*PostingList)->df++;
    }
}

int getTermFrequency(PostingList *postingList, int id) {
    if (postingList == NULL) {
        return 0;
    }
    ListNode *current = postingList->first;
    while (current != NULL) {       ///
        if (current->id_times[0] == id) {
            return current->id_times[1];
        }
        current = current->next;
    }
    return 0;
}

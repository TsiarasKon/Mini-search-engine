#include <stdio.h>
#include <stdlib.h>
#include "postinglist.h"

ListNode* createListNode(int id) {
    ListNode *listNode = malloc(sizeof(ListNode));
    if (listNode == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }
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
    if (postingList == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }
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

int incrementPostingList(TrieNode *node, int id) {
    PostingList **PostingList = &node->postingList;
    if ((*PostingList)->first == NULL) {
        (*PostingList)->first = createListNode(id);
        if ((*PostingList)->first == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            return 4;
        }
        (*PostingList)->last = (*PostingList)->first;
        (*PostingList)->df++;
        return 0;
    }
    if ((*PostingList)->last->id_times[0] == id) {   // word belongs to the same (last) doc
        (*PostingList)->last->id_times[1]++;
    } else {
        (*PostingList)->last->next = createListNode(id);
        if ((*PostingList)->last->next == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            return 4;
        }
        (*PostingList)->last = (*PostingList)->last->next;
        (*PostingList)->df++;
    }
    return 0;
}

int getTermFrequency(PostingList *postingList, int id) {
    if (postingList == NULL) {
        return 0;
    }
    ListNode *current = postingList->first;
    while (current != NULL && current->id_times[0] <= id) {
        if (current->id_times[0] == id) {
            return current->id_times[1];
        }
        current = current->next;
    }
    return 0;
}

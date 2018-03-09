#include <stdio.h>
#include <stdlib.h>

#include "pairingheap.h"

HeapNode* createHeapNode(double score, int id) {
    HeapNode *heapNode = malloc(sizeof(HeapNode));
    heapNode->score = score;
    heapNode->id = id;
    heapNode->sibilings = NULL;
    heapNode->children = NULL;
    return heapNode;
}

void destroyHeap(HeapNode **heapNode) {
    if (*heapNode == NULL) {
        fprintf(stderr, "Attempted to destroy a NULL HeapNode.\n");
        return;
    }
    if ((*heapNode)->children != NULL) {
        destroyHeap(&(*heapNode)->children);
        (*heapNode)->children = NULL;
    }
    if ((*heapNode)->sibilings != NULL) {
        destroyHeap(&(*heapNode)->sibilings);
        (*heapNode)->sibilings = NULL;
    }
    free(*heapNode);
    *heapNode = NULL;
//    while (*heapNode != NULL) {
//        *heapNode = deleteMaxNode(heapNode);
//    }
}

void addHeapChild(HeapNode *heap, HeapNode *heapNode) {
    HeapNode *nextChild = heap->children;
    heap->children = heapNode;
    heapNode->sibilings = nextChild;
}

HeapNode* heapMerge(HeapNode *heap1, HeapNode *heap2) {
    if (heap2 == NULL) {
        return heap1;
    } else if (heap1 == NULL) {
        return heap2;
    } else if (heap1->score > heap2->score) {
        addHeapChild(heap1, heap2);
        return heap1;
    } else {        // (heap1->score <= heap2->score)
        addHeapChild(heap2, heap1);
        return heap2;
    }
}

HeapNode* mergePairs(HeapNode *children) {
    if (children == NULL || children->sibilings == NULL) {
        return children;
    }
    HeapNode *heap1, *heap2, *newNode;
    heap1 = children;
    heap2 = children->sibilings;
    newNode = children->sibilings->sibilings;
    heap1->sibilings = NULL;
    heap2->sibilings = NULL;
    return heapMerge(heapMerge(heap1, heap2), mergePairs(newNode));
}

HeapNode* heapInsert(HeapNode *heap, double score, int id) {
    return heapMerge(heap, createHeapNode(score, id));
}

HeapNode* deleteMaxNode(HeapNode **heap) {
    if (*heap == NULL) {
        return NULL;
    }
    HeapNode* children = (*heap)->children;
    free(*heap);
    return mergePairs(children);
}
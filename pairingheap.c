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

void destroyHeap(HeapNode *heapNode) {
    if (heapNode == NULL) {
        fprintf(stderr, "Attempted to destroy a NULL HeapNode.\n");
        return;
    }
    if (heapNode->children != NULL) {
        destroyHeap(heapNode->children);
    }
    if (heapNode->sibilings != NULL) {
        destroyHeap(heapNode->sibilings);
    }
    free(heapNode);
}

void addHeapChild(HeapNode *heap, HeapNode *heapNode) {
    HeapNode *nextChild = heap->children;
    heap->children = heapNode;
    heapNode->sibilings = nextChild;
}

//HeapNode* heapify(HeapNode *heap) {
//    if (heap->sibilings != NULL) {
//        //addHeapChild(heap, heap->sibilings);
//        heap->sibilings = NULL;
//    }
//    return heap;
//}

HeapNode* heapMerge(HeapNode *heap1, HeapNode *heap2) {
    if (heap2 == NULL) {
        return heap1;
    } else if (heap1 == NULL) {
        return heap2;
    } else if (heap1->score > heap2->score) {
        addHeapChild(heap1, heap2);
        return heap1;
    } else if (heap1->score < heap2->score) {
        addHeapChild(heap2, heap1);
        return heap2;
    }
}

HeapNode* mergePairs(HeapNode *children) {
    if (children == NULL || children->sibilings == NULL) {
        return children;
    }
    return heapMerge(heapMerge(children, children->sibilings), mergePairs(children->sibilings->sibilings));
}

HeapNode* heapInsert(HeapNode *heap, double score, int id) {
    heapMerge(heap, createHeapNode(score, id));
}

HeapNode* deleteMaxNode(HeapNode *heap) {
    if (heap == NULL) {
        fprintf(stderr, "Attempted to delete a NULL HeapNode.\n");
        return NULL;
    }
    HeapNode *res = heap->children;
    free(heap);
    return mergePairs(res);
}
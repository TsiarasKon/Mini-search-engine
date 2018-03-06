#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trie.h"

TrieNode* createTrieNode(char value, TrieNode *next) {
    TrieNode* nptr = malloc(sizeof(TrieNode));
    nptr->value = value;
    nptr->next = next;
    nptr->child = NULL;
    nptr->postingList = createPostingList();
    return nptr;
}

void deleteTrieNode(TrieNode *trieNode) {
    if (trieNode == NULL) {
        fprintf(stderr, "Attempted to delete a NULL TrieNode.\n");
        return;
    }
    deletePostingList(trieNode->postingList);
    if (trieNode->child != NULL) {
        deleteTrieNode(trieNode->child);
    }
    if (trieNode->next != NULL) {
        deleteTrieNode(trieNode->next);
    }
    free(trieNode);
}

Trie* createTrie() {
    Trie* tptr = malloc(sizeof(Trie));
    tptr->first = NULL;
    return tptr;
}

void deleteTrie(Trie *trie) {
    if (trie == NULL) {
        fprintf(stderr, "Attempted to delete a NULL Trie.\n");
        return;
    }
    if (trie->first != NULL) {
        deleteTrieNode(trie->first);
    }
    free(trie);
}

void directInsert(TrieNode *current, char *word, int id, int i) {
    while (i < strlen(word) - 1) {
        current->child = createTrieNode(word[i], NULL);
        current = current->child;
        i++;
    }
    current->child = createTrieNode(word[i], NULL);       // final letter
    incrementPostingList(current->child, id);
}

void insert(Trie *root, char *word, int id) {
    if (word[0] == '\0') {      // word is an empty string
        return;
    }
    size_t wordlen = strlen(word);
    if (root->first == NULL) {      // only in first Trie insert
        root->first = createTrieNode(word[0], NULL);
        if (wordlen == 1) {     // just inserted the final letter
            incrementPostingList(root->first, id);
            return;
        }
        directInsert(root->first, word, id, 1);
        return;
    }
    TrieNode **current = &root->first;
    for (int i = 0; i < wordlen; i++) {
        while (word[i] > (*current)->value) {
            if ((*current)->next == NULL) {
                (*current)->next = createTrieNode(word[i], NULL);
            }
            current = &(*current)->next;
        }
        if ((*current) != NULL && word[i] < (*current)->value) {        // must be inserted before
            TrieNode *newNode = createTrieNode(word[i], (*current));
            *current = newNode;
        }
        // Go to next letter:
        if (i == wordlen - 1) {     // just inserted the final letter
            incrementPostingList((*current), id);
        } else if ((*current)->child != NULL) {     // proceed to child
            current = &(*current)->child;
        } else {    // child doesn't exist so we just add the entire word in the trie
            directInsert((*current), word, id, i + 1);
            return;
        }
    }
}

PostingList *getPostingList(Trie *root, char *word) {
    size_t wordlen = strlen(word);
    if (root->first == NULL) {       // empty Trie
        return NULL;
    }
    TrieNode *current = root->first;
    for (int i = 0; i < wordlen; i++) {
        while (word[i] > current->value) {
            if (current->next == NULL) {
                return NULL;
            }
            current = current->next;
        }
        if (word[i] < current->value) {        // must be inserted before
            return NULL;
        }
        // Go to next letter:
        if (i == wordlen - 1) {     // word found
            return current->postingList;
        } else if (current->child != NULL) {     // proceed to child
            current = current->child;
        } else {            // child doesn't exist
            return NULL;
        }
    }
    return NULL;
}

void printTrieNode(TrieNode *node, char *prefix) {
    TrieNode *currentChild = node->child;
    size_t prefixLen = strlen(prefix);
    char *word = malloc(prefixLen + 2);
    strcpy(word, prefix);
    word[prefixLen + 1] = '\0';
    while (currentChild != NULL) {
        word[prefixLen] = currentChild->value;
        if (currentChild->postingList->first != NULL) {
            printf("%s %d\n", word, currentChild->postingList->df);
        }
        printTrieNode(currentChild, word);
        currentChild = currentChild->next;
    }
    free(word);
}

void printTrie(Trie *root) {
    TrieNode *current = root->first;
    char first_letter[2];
    first_letter[1] = '\0';
    while (current != NULL) {
        if (current->postingList->first != NULL) {
            printf("%c %d\n", current->value, current->postingList->df);
        }
        first_letter[0] = current->value;
        printTrieNode(current, first_letter);
        current = current->next;
    }
}

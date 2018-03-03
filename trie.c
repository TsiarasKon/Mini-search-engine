#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trie.h"

TrieNode* createTrieNode(char value) {
    TrieNode* nptr = malloc(sizeof(TrieNode));
    nptr->value = value;
    nptr->next = NULL;
    nptr->child = NULL;
    nptr->postingList = createPostingList();
    return nptr;
}

Trie* createTrie() {
    Trie* tptr = malloc(sizeof(Trie));
    tptr->first = NULL;
    return tptr;
}

/// free all

void directInsert(TrieNode *current, char *word, int id, int i) {
    while (i < strlen(word) - 1) {
        current->child = createTrieNode(word[i]);
        current = current->child;
        i++;
    }
    current->child = createTrieNode(word[i]);       // final letter
    incrementPostingList(current->child, id);
}

void insert(Trie *root, char *word, int id) {
    if (word[0] == '\0') {      // word is an empty string
        return;
    }
    int wordlen = strlen(word);
    if (root->first == NULL) {      // only in first Trie insert
        root->first = createTrieNode(word[0]);
        if (wordlen == 1) {     // just inserted the final letter
            incrementPostingList(root->first, id);
            return;
        }
        directInsert(root->first, word, id, 1);
        return;
    }
    TrieNode *current = root->first;
    for (int i = 0; i < wordlen; i++) {
        while (word[i] != current->value) {
            if (current->next == NULL) {
                current->next = createTrieNode(word[i]);
            }
            current = current->next;
        }
        if (i == wordlen - 1) {     // just inserted the final letter
            incrementPostingList(current, id);
        } else if (current->child != NULL) {     // proceed to child
            current = current->child;
        } else {    // child doesn't exist so we just add the entire word in the trie
            directInsert(current, word, id, i + 1);
            return;
        }
    }
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
            printf("%s\n", word);
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
            printf("%c\n", current->value);
        }
        first_letter[0] = current->value;
        printTrieNode(current, first_letter);
        current = current->next;
    }
}

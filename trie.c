#include <stdlib.h>
#include <string.h>
#include "trie.h"

Trie* createTrie() {
    Trie* tptr = malloc(sizeof(Trie));
    tptr->first = NULL;
    return tptr;
}

Node* createNode(char value) {
    Node* nptr = malloc(sizeof(Node));
    nptr->value = value;
    nptr->next = NULL;
    nptr->child = NULL;
    nptr->postingList = NULL;
    return nptr;
}

void incrementPostingList(Node *node, int id) {
    Listnode *current = node->postingList;
    while (current != NULL) {
        if (current->id_times[0] == id) {
            current->id_times[1]++;
            return;
        }
        current = current->next;
    }
    Listnode *newPostingList = malloc(sizeof(Listnode));
    newPostingList->id_times[0] = id;
    newPostingList->id_times[1]++;
    newPostingList->next = NULL;
    current = newPostingList;
}

/// free all

void directInsert(Node *current, char *word, int id, int i) {
    while (i < strlen(word) - 1) {
        current->child = createNode(word[i]);
        current = current->child;
        i++;
    }
    current->child = createNode(word[i]);       // final letter
    incrementPostingList(current->child, id);
}

void insert(Trie *root, char *word, int id) {
    if (word[0] == '\0') {      // word is an empty string
        return;
    }
    int wordlen = strlen(word);
    Node *current = root->first;
    if (current == NULL) {      // only in first Trie insert
        current = createNode(word[0]);
        if (wordlen == 1) {     // just inserted the final letter
            incrementPostingList(current, id);
            return;
        }
        directInsert(current, word, id, 1);
        return;
    }
    for (int i = 0; i < wordlen; i++) {
        while (word[i] != current->value) {
            current = current->next;
            if (current->next == NULL) {
                current->next = createNode(word[i]);
                current = current->next;
            }
        }
        if (i == wordlen - 1) {     // just inserted the final letter
            incrementPostingList(current, id);
        } else if (current->child != NULL) {     // proceed to child
            current = current->child;
        } else {    // child doesn't exist so we just add the entire word in the trie
            directInsert(current, word, id, i);
            return;
        }
    }
}
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trie.h"

TrieNode* createTrieNode(char value, TrieNode *next) {
    TrieNode* nptr = malloc(sizeof(TrieNode));
    if (nptr == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }
    nptr->value = value;
    nptr->next = next;
    nptr->child = NULL;
    nptr->postingList = createPostingList();
    return nptr;
}

void deleteTrieNode(TrieNode **trieNode) {
    if (trieNode == NULL) {
        fprintf(stderr, "Attempted to delete a NULL TrieNode.\n");
        return;
    }
    deletePostingList(&(*trieNode)->postingList);
    if ((*trieNode)->child != NULL) {
        deleteTrieNode(&(*trieNode)->child);
    }
    if ((*trieNode)->next != NULL) {
        deleteTrieNode(&(*trieNode)->next);
    }
    free(*trieNode);
    *trieNode = NULL;
}

Trie* createTrie() {
    Trie* tptr = malloc(sizeof(Trie));
    if (tptr == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }
    tptr->first = NULL;
    return tptr;
}

void deleteTrie(Trie **trie) {
    if (*trie == NULL) {
        fprintf(stderr, "Attempted to delete a NULL Trie.\n");
        return;
    }
    if ((*trie)->first != NULL) {
        deleteTrieNode(&(*trie)->first);
    }
    free(*trie);
    *trie = NULL;
}

int directInsert(TrieNode *current, char *word, int id, int i) {
    while (i < strlen(word) - 1) {
        current->child = createTrieNode(word[i], NULL);
        if (current->child == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            return 4;
        }
        current = current->child;
        i++;
    }
    current->child = createTrieNode(word[i], NULL);       // final letter
    if (current->child == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 4;
    }
    return incrementPostingList(current->child, id);
}

int insert(Trie *root, char *word, int id) {
    if (word[0] == '\0') {      // word is an empty string
        return 0;
    }
    size_t wordlen = strlen(word);
    if (root->first == NULL) {      // only in first Trie insert
        root->first = createTrieNode(word[0], NULL);
        if (wordlen == 1) {     // just inserted the final letter
            incrementPostingList(root->first, id);
            return 0;
        }
        return directInsert(root->first, word, id, 1);
    }
    TrieNode **current = &root->first;
    for (int i = 0; i < wordlen; i++) {
        while (word[i] > (*current)->value) {
            if ((*current)->next == NULL) {
                (*current)->next = createTrieNode(word[i], NULL);
                if ((*current)->next == NULL) {
                    fprintf(stderr, "Failed to allocate memory.\n");
                    return 4;
                }
            }
            current = &(*current)->next;
        }
        if ((*current) != NULL && word[i] < (*current)->value) {        // must be inserted before
            TrieNode *newNode = createTrieNode(word[i], (*current));
            if (newNode == NULL) {
                fprintf(stderr, "Failed to allocate memory.\n");
                return 4;
            }
            *current = newNode;
        }
        // Go to next letter:
        if (i == wordlen - 1) {     // just inserted the final letter
            return incrementPostingList((*current), id);
        } else if ((*current)->child != NULL) {     // proceed to child
            current = &(*current)->child;
        } else {    // child doesn't exist so we just add the entire word in the trie
            return directInsert((*current), word, id, i + 1);
        }
    }
    return 0;
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

int printTrieNode(TrieNode *node, char *prefix) {
    TrieNode *currentChild = node->child;
    size_t prefixLen = strlen(prefix);
    char *word = malloc(prefixLen + 2);
    if (word == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 4;
    }
    strcpy(word, prefix);
    word[prefixLen + 1] = '\0';
    int exit_code;
    while (currentChild != NULL) {
        word[prefixLen] = currentChild->value;
        if (currentChild->postingList->first != NULL) {
            printf("%s %d\n", word, currentChild->postingList->df);
        }
        exit_code = printTrieNode(currentChild, word);
        if (exit_code > 0) {
            return exit_code;
        }
        currentChild = currentChild->next;
    }
    free(word);
    return 0;
}

int printTrie(Trie *root) {
    TrieNode *current = root->first;
    char first_letter[2];
    first_letter[1] = '\0';
    int exit_code;
    while (current != NULL) {
        if (current->postingList->first != NULL) {
            printf("%c %d\n", current->value, current->postingList->df);
        }
        first_letter[0] = current->value;
        exit_code = printTrieNode(current, first_letter);
        if (exit_code > 0) {
            return exit_code;
        }
        current = current->next;
    }
    return 0;
}

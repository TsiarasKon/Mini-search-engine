#ifndef TRIE_H
#define TRIE_H

typedef struct listnode{
    int id_times[2];
    struct listnode *next;
} Listnode;

typedef struct node{
    char value;
    struct node *next;
    struct node *child;
    Listnode *postingList;
} Node;

typedef struct {
    Node *first;
} Trie;

Trie* createTrie();
Node* createNode(char value);
void incrementPostingList(Node *node, int id);

void directInsert(Node *current, char *word, int id, int i);
void insert(Trie *root, char *word, int id);


#endif

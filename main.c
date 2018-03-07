#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
#include "util.h"
#include "pairingheap.h"

void interface(Trie *trie, int K, char **docs, int doc_count);

int main(int argc, char *argv[]) {
    char *docfile = NULL;
    int K = 10;     // default number of results
    for (int i = 1; i < argc; i += 2) {
        if (!strcmp(argv[i], "-i")) {
            if (i == argc) {
                docfile = NULL;
                break;
            }
            docfile = argv[i + 1];
        } else if (!strcmp(argv[i], "-k")) {
            if (i == argc) {
                docfile = NULL;
                break;
            }
            K = atoi(argv[i + 1]);
            if (K < 1) {
                fprintf(stderr, "Invalid arguments: K must be greater than 0.\n");
                return 1;
            }
        } else {
            docfile = NULL;
            break;
        }
    }
    if (docfile == NULL) {      // docfile was not given and/or unexpected arguments were encountered
        fprintf(stderr, "Invalid arguments: Please run \"$ ./minisearch -i docfile -k K\"\n");
        return 1;
    }

    int doc_count = 0;
    FILE *fp = fopen(docfile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open '%s'.\n", docfile);
        return 2;
    }
    printf("Loading docs from '%s'...\n", docfile);
    size_t bufsize = 128;      // sample size - getline will reallocate memory as needed
    char *buffer = malloc(bufsize);
    char *bufferptr = buffer;       // used to free buffer in the end, since we're using strtok
    for (int i = 0; ; i++) {
        if (getline(&buffer, &bufsize, fp) == -1) {
            break;
        }
        while (*buffer == ' ' || *buffer == '\t') {
            buffer++;
        }
        if (atoi(buffer) != i) {
            fprintf(stderr, "Error in '%s' - Docs not in order.\n", docfile);
            return 3;
        }
        doc_count++;
    }
    rewind(fp);     // start again from the beginning of docfile

    Trie* trie = createTrie();
    char **docs = malloc(doc_count * sizeof(char*));
    char *word;
    for (int id = 0; id < doc_count; id++) {
        if (getline(&buffer, &bufsize, fp) == -1) {
            fprintf(stderr, "Something unexpected happened.\n");
            return -1;
        }
        while (*buffer == ' ' || *buffer == '\t') {
            buffer++;
        }
        strtok(buffer, "\r\n");         // remove trailing newline character
        docs[id] = malloc(strlen(buffer) + 1);
        strcpy(docs[id], buffer);
        strtok(buffer, " \t");          // skip index number in front of doc
        word = strtok(NULL, " \t");     // get first word
        while (word != NULL) {          // for every word in doc
            insert(trie, word, id);
            word = strtok(NULL, " \t");
        }
    }
    free(bufferptr);
    printf("Docs loaded successfully!\n");

    interface(trie, K, docs, doc_count);

    deleteTrie(trie);
    for (int i = 0; i < doc_count; i++) {
        free(docs[i]);
    }
    free(docs);
    return 0;
}



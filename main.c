#define _GNU_SOURCE         // for getline()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"
#include "util.h"
#include "pairingheap.h"

int K = 10;     // default number of results
int doc_count = 0;
double avgdl = 0.0;

void interface(Trie *trie, char **docs, int *docWc);

int main(int argc, char *argv[]) {
    char *docfile = NULL;
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

    FILE *fp = fopen(docfile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open '%s'.\n", docfile);
        return 2;
    }
    printf("Loading docs from '%s'...\n", docfile);
    size_t bufsize = 128;      // sample size - getline will reallocate memory as needed
    char *buffer = NULL, *bufferptr = NULL;
    for (int i = 0; ; i++) {
        if (getline(&buffer, &bufsize, fp) == -1) {
            break;
        }
        bufferptr = buffer;
        while (*buffer == ' ' || *buffer == '\t') {
            buffer++;
        }
        if (atoi(buffer) != i) {
            fprintf(stderr, "Error in '%s' - Docs not in order.\n", docfile);
            return 3;
        }
        doc_count++;
        buffer = bufferptr;     // resetting buffer that maybe was moved
    }
    rewind(fp);     // start again from the beginning of docfile

    Trie* trie = createTrie();
    char *docs[doc_count];
    char *word;
    int docWc[doc_count];
    for (int id = 0; id < doc_count; id++) {
        docWc[id] = 0;
        if (getline(&buffer, &bufsize, fp) == -1) {
            fprintf(stderr, "Something unexpected happened.\n");
            return -1;
        }
        bufferptr = buffer;
        while (*buffer == ' ' || *buffer == '\t') {         // ignore whitespace before id
            buffer++;
        }
        while (isdigit(*buffer)) {          // ignore the id itself
            buffer++;
        }
        strtok(buffer, "\r\n");         // remove trailing newline character
        docs[id] = malloc(strlen(buffer) + 1);
        strcpy(docs[id], buffer);
        word = strtok(buffer, " \t");     // get first word
        while (word != NULL) {          // for every word in doc
            insert(trie, word, id);
            docWc[id]++;
            avgdl++;
            word = strtok(NULL, " \t");
        }
        buffer = bufferptr;     // resetting buffer that maybe was moved
    }
    if (bufferptr != NULL) {
        free(bufferptr);
    }
    fclose(fp);
    printf("Docs loaded successfully!\n");

    interface(trie, docs, docWc);

    deleteTrie(&trie);
    for (int i = 0; i < doc_count; i++) {
        free(docs[i]);
    }
    return 0;
}



OBJS 	= main.o interface.o pairingheap.o postinglist.o trie.o util.o
SOURCE	= main.c interface.c pairingheap.c postinglist.c trie.c util.c
HEADER  = pairingheap.h postinglist.h trie.h util.h
OUT  	= sysproEx1
CC		= gcc
FLAGS   = -g3 -c -pedantic -std=c99 -Wall

$(OUT): $(OBJS)
	$(CC) -g3 $(OBJS) -o $@ -lm

main.o: main.c
	$(CC) $(FLAGS) main.c

interface.o: interface.c
	$(CC) $(FLAGS) interface.c

pairingheap.o: pairingheap.c
	$(CC) $(FLAGS) pairingheap.c

postinglist.o: postinglist.c
	$(CC) $(FLAGS) postinglist.c

trie.o: trie.c
	$(CC) $(FLAGS) trie.c

util.o: util.c
	$(CC) $(FLAGS) util.c -lm


clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

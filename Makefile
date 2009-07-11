CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -DTRIE_TEST

ifeq ($(debug),1)
	CFLAGS += -DDEBUG -g
endif

all: trie.h
	$(CC) $(CFLAGS) -o trie trie.c

clean:
	$(RM) *.o trie

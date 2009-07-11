#ifndef PI_TRIE_H
#define PI_TRIE_H

#include <stddef.h>

enum { TRIE_NODE_LOW_CHILD=0, TRIE_NODE_HIGH_CHILD=1, TRIE_NODE_EQ_CHILD=2 };

/* This trie is a ternary tree. */
typedef struct _TRIE_NODE {
    struct _TRIE_NODE* relatives[ 3 ];
    void* data;
    char  splitchar;
} TRIE_NODE;

typedef struct _TRIE {
    TRIE_NODE* nodeList;  /* Array of TRIE_NODEs */
    size_t nodeListSize;  /* Array size */
    size_t nodeListIndex; /* Next unallocated node in nodeList */
    TRIE_NODE* root;
} TRIE;

TRIE* Trie_new( size_t maxSize );
void Trie_free( TRIE* self );

void* Trie_Lookup( TRIE* self, const char* key );
int Trie_Insert( TRIE* self, const char* key, void* data );

#endif /* PI_TRIE_H */

/*
 * A basic trie implementation. This trie takes the form of a ternary
 * search tree.
 *
 * References:
 * http://en.wikipedia.org/wiki/Ternary_search_tree
 * http://www.javaworld.com/javaworld/jw-02-2001/jw-0216-ternary.html?page=1
 *
 * High-level goals:
 *  - Fast lookup
 *  - Simple as possible
 *  - No per node heap allocation
 *
 * In this implementation, key removal is not necessary and as such,
 * is not implemented.
 */

#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#  define DBG_PRINTF(fmt, ...) printf( fmt, ## __VA_ARGS__ )
#else
#  define DBG_PRINTF(fmt, ...) do { } while (0)
#endif

TRIE_NODE* TrieNode_init( TRIE_NODE* node, char splitchar, void* data )
{
    node->splitchar = splitchar;
    node->data = data;
    node->relatives[ TRIE_NODE_LOW_CHILD ] = NULL;
    node->relatives[ TRIE_NODE_HIGH_CHILD ] = NULL;
    node->relatives[ TRIE_NODE_EQ_CHILD ] = NULL;

    return node;
}

TRIE* Trie_new( size_t maxSize )
{
    TRIE* trie;

    /* We need at least 1 nodes for this thing to work. */
    if ( maxSize < 1 )
        maxSize = 1;

    trie = malloc( sizeof( TRIE ) );
    if ( trie != NULL ) {
        size_t i;
        trie->nodeList = malloc( sizeof( TRIE_NODE ) * maxSize );
        if ( trie->nodeList == NULL ) {
            free( trie );
            return NULL;
        }
        trie->nodeListSize = maxSize;
        /* Index 0 is reserved, next available is index 1 */
        trie->nodeListIndex = 1;
        for ( i = 0; i < maxSize; i++ ) {
            TrieNode_init( &(trie->nodeList[ i ]), 0, NULL );
        }
        trie->root = &trie->nodeList[ 0 ];
    }
    return trie;
}

void Trie_free( TRIE* self )
{
    if ( self ) {
        free( self->nodeList );
        self->nodeListIndex = self->nodeListSize = 0;
        free( self );
    }
}

/* Basic lookup algorithm from
   http://www.javaworld.com/javaworld/jw-02-2001/jw-0216-ternary.html */
void* Trie_Lookup( TRIE* self, const char* key )
{
    TRIE_NODE* curr = self->root;
    int keyIndex = 0;

    while ( 1 ) {
        if ( curr == NULL )
            return NULL;
        if ( key[ keyIndex ] == curr->splitchar ) {
            keyIndex += 1;
            if ( key[ keyIndex ] == '\0' )
                return curr->data;
            else
                curr = curr->relatives[ TRIE_NODE_EQ_CHILD ];
        }
        else {
            int index = key[ keyIndex ] < curr->splitchar ?
                    TRIE_NODE_LOW_CHILD : TRIE_NODE_HIGH_CHILD;
            curr = curr->relatives[ index ];
        }
    }
}

int Trie_Insert( TRIE* self, const char* key, void* data )
{
    TRIE_NODE** curr = &self->root;
    int keyIndex = 0;
    int keyLength = strlen( key );
    char currentChar = key[ 0 ];

    DBG_PRINTF( "inserting {\"%s\": %p} at %p\n", key, data, (void*)*curr );
    while ( 1 ) {
        if ( *curr == NULL ) {
            DBG_PRINTF( "  allocating a new node for \"%c\"... ", currentChar );
            if ( self->nodeListIndex >= self->nodeListSize ) {
                DBG_PRINTF( "out of memory!\n" );
                return -1;
            }
            self->nodeListIndex += 1;
            *curr = &( self->nodeList[ self->nodeListIndex ] );
            TrieNode_init( *curr, currentChar, NULL );
            DBG_PRINTF( "okay, new node is %p\n", (void*)*curr );
            if ( keyIndex == keyLength - 1 ) {
                DBG_PRINTF( "  End of key at node %p\n", (void*)*curr );
                (*curr)->data = data;
                break;
            }
        }
        if ( currentChar == (*curr)->splitchar ) {
            DBG_PRINTF( "  found character match on \"%c\"\n", currentChar );
            keyIndex += 1;
            currentChar = key[ keyIndex ];
            curr = &(*curr)->relatives[ TRIE_NODE_EQ_CHILD ];
        }
        else {
            int index = currentChar < (*curr)->splitchar ?
                    TRIE_NODE_LOW_CHILD : TRIE_NODE_HIGH_CHILD;
            DBG_PRINTF( "  \"%c\" does not match \"%c\"\n", currentChar, (*curr)->splitchar );
            curr = &(*curr)->relatives[ index ];
        }
    }
    return 0;
}

#ifdef TRIE_TEST
#include <assert.h>

int main()
{
    int n = 2142;
    int* pn;
    char c = 'R';
    char* pc;
    int insertOK;

    TRIE* testTrie = Trie_new( 100 );

    insertOK = Trie_Insert( testTrie, "foo", &n );
    assert( insertOK != -1 );
    pn = Trie_Lookup( testTrie, "foo" );
    assert( pn != NULL );
    printf( "n: %d; *pn: %d\n", n, *pn );
    assert( n == *pn );

    insertOK = Trie_Insert( testTrie, "edgar", &c );
    assert( insertOK != -1 );
    pc = Trie_Lookup( testTrie, "edgar" );
    assert( pc != NULL );
    printf( "c: %c; *pc: %c\n", c, *pc );
    assert( c == *pc );

    c = 'T';
    insertOK = Trie_Insert( testTrie, "for", &c );
    assert( insertOK != -1 );
    pc = Trie_Lookup( testTrie, "for" );
    assert( pc != NULL );
    printf( "c: %c; *pc: %c\n", c, *pc );
    assert( c == *pc );

    pn = Trie_Lookup( testTrie, "foo" );
    assert( pn != NULL );
    printf( "n: %d; *pn: %d\n", n, *pn );
    assert( n == *pn );

    Trie_free( testTrie );

    return 0;
}
#endif

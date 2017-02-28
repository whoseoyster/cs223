#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "/c/cs223/hw3/hash.h"


struct elt {
  struct elt *next;
  char *key;
};

struct hash {
  int size;           // size of the pointer table 
  int n;              // number of elements stored 
  int buckets;
  struct elt **table;
};

/* Hash initialization code used in both HashCreate and grow */
Hash
internalHashCreate(int size)
{
    Hash h;
    int i;

    h = malloc(sizeof(*h));

    assert(h != 0);

    h->size = size;
    h->n = 0;
    h->buckets = 0;
    h->table = malloc(sizeof(struct elt *) * h->size);

    assert(h->table != 0);

    for(i = 0; i < h->size; i++) h->table[i] = 0;

    return h;
}

Hash
HashCreate(void)
{
    return internalHashCreate(INITIAL_SIZE);
}

void
HashDestroy(Hash d)
{
    int i;
    struct elt *e;
    struct elt *next;

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->key);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define MULTIPLIER (97)

static unsigned long
hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h;

    h = 0;

    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

static void
grow(Hash d)
{
    Hash d2;            /* new Hash we'll create */
    struct hash swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

	int n=d->size;
    int size=n*GROWTH_FACTOR;
    int buckets=d->buckets;
    float occ= buckets/(float)n;

    if(debugflag)
    	printf("Growing to size: %i. n: %i. Used buckets: %i. Occupancy rate: %.2f\n", size, n, buckets, occ);

    d2 = internalHashCreate(d->size * GROWTH_FACTOR);

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            /* note: this recopies everything */
            /* a more efficient implementation would
             * patch out the strdups inside HashInsert
             * to avoid this problem */
            HashInsert(d2, e->key);
        }
    }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call HashDestroy on d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;

    HashDestroy(d2);
}

/* insert a new key into an existing Hash table */
void
HashInsert(Hash d, const char *key)
{
	if (HashSearch(d, key))
		return;
    struct elt *e;
    unsigned long h;

    assert(key);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);

    h = hash_function(key) % d->size;

    if (d->table[h] == 0) {
    	d->buckets++;
    }

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
bool
HashSearch(Hash d, const char *key)
{
    struct elt *e;

    for(e = d->table[hash_function(key) % d->size]; e != 0; e = e->next) {
        if(!strcmp(e->key, key)) {
            /* got it */
            return true;
        }
    }

    return false;
}

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void
HashDelete(Hash d, const char *key)
{
    struct elt **prev;          /* what to change when elt is deleted */
    struct elt *e;              /* what to delete */

    for(prev = &(d->table[hash_function(key) % d->size]); 
        *prev != 0; 
        prev = &((*prev)->next)) {
        if(!strcmp((*prev)->key, key)) {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e);

            return;
        }
    }
}

void
HashDisplay(Hash d)
{
	struct elt *e;
	for(int i=0; i<d->size; i++) {
		if(d->table[i] != NULL) {
			printf("[%i] ", i);

		    for(e = d->table[i]; e != 0; e = e->next) {
		        printf("(%s) ", e->key);
		    }
		    printf("\n");
		}
	}
}

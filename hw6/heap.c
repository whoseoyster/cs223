#include "heap.h"
#include "dict.h"
#include <assert.h>

#define MAXHEAP 4

// heapslot * heap;
// int heapnum;
// int maxheap;

// // from Van Wyk, chapter 10, page 233++
// typedef struct heapslot heapslot;
// struct heapslot {
//   int key;
//   struct room * value;
// } ;

// struct heap {
//     int size;           /* size of the pointer table */
//     int n;              /* number of elements stored */
//     struct heapslot ** heap;
// };

// typedef struct heap *Heap;

bool empty(Heap h)
{
	return h->n == 0;
}

void demand(int cond, char * msg)
{
	if (! cond) {
    	fprintf(stderr, "%s", msg);
    	exit(1);
  	}
  	return;
}

void printHeap(Heap h)
{
	int heapcount = h->n;
	printf("HEAP: ");
	for(int i = 1; i <= heapcount; i++){
		printf("[%d: %s %d/%d] ", i, h->heap[i]->value->room, h->heap[i]->key, h->heap[i]->value->temp);
	}
	printf("\n");
}

struct room * findmin(Heap h)
{
	demand( !empty(h), "findmin not allowed on empty heap\n");
  	return h->heap[1]->value;
}

Heap initheap()
{
	// heapnum = 0;
	int maxheap = MAXHEAP;
	// heap = malloc(maxheap * sizeof(heap));
	// heap[0].key = INT_MIN;

	Heap h;
	h = malloc(sizeof(*h));
	assert(h);

	h->n = 0;
	h->size = maxheap;
	h->heap = malloc(sizeof(struct heapslot *) * maxheap);

	assert(h->heap);

	for(int i = 0; i < h->size; i++) h->heap[i] = 0;

	// struct heapslot *i;
	// i = h->heap[0];
	// i->key = 0;

	heapslot *e;

    e = malloc(sizeof(*e));

    assert(e);

    e->key = INT_MIN;
    e->value = 0;
    h->heap[0] = e;

	return h;
}

void swap(heapslot *s1, heapslot *s2)
{
	heapslot temp;
	printf("SWAP: %d <=> %d\n", s1->key, s2->key);
	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void insert( Heap h, int key, struct room * r)
{
	int cur, parent;

	printf("INSERT: %d, %s, %d ", key, r->room, r->temp);
	fflush(stdout); 

	int heapnum = h->n;
	int maxheap = h->size;

	//demand(heapnum < maxheap, "heap overflow\n");
	while(heapnum  >= maxheap) {
		maxheap *= 2;
		printf("\nExpanding heap size to %d\n", maxheap);
		h = realloc(h, sizeof(*h) * maxheap);
		demand(h != 0, "malloc fail in insert\n");
	}

	heapslot *e;

    assert(r);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = key;
    e->value = r;
    //printRoom(value);

	cur = ++heapnum;

	h->heap[cur] = e;

	parent = cur/2;

	while (h->heap[parent] && h->heap[parent]->key > h->heap[cur]->key) {
		printf("\n--");
		printHeap(h);
		demand(parent > 0, "inserted item rising past root\n");
		swap(h->heap[parent], h->heap[cur]);
		cur = parent;
		parent = cur/2;
	}

	h->n++;

	printHeap(h);
}

void deletemin( Heap h)
{
	int cur, child;
	int heapnum = h->n;
	printf("DELETEMIN: ");
	demand(! empty(h), "deletemin not allowed on empty heap\n");
	h->heap[1] = h->heap[heapnum--];
	cur = 1;
	child = 2;
	while (child <= heapnum) {
		printf("\n--");
		printHeap(h);
		if (child < heapnum &&
			h->heap[child+1]->key < h->heap[child]->key)
		  child++;
		if (h->heap[cur]->key > h->heap[child]->key) {
		  demand(child <= heapnum, "falling past leaves\n");
		  swap(h->heap[cur], h->heap[child]);
		  cur = child;
		  child = 2*cur;
		} else
		  break;
	}
	printHeap(h);
}

void destroyHeap( Heap h)
{
	int i;
    heapslot *e;

    printf("DestroyHeap\n");

    for(i = 0; i <= h->n; i++) {
        e = h->heap[i];
        printf("Freeing: %d\n", e->key);
        if (e->value)
        	free(e->value);
        free(e);
    }

    free(h->heap);
    free(h);
}

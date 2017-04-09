#include "heap.h"
#include "dict.h"
#include <assert.h>


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
	fflush(stdout);
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
	int maxheap = MAXHEAP;

	Heap h;
	h = malloc(sizeof(*h));
	assert(h);

	h->n = 0;
	h->size = maxheap;
	h->heap = malloc(sizeof(struct heapslot *) * maxheap);

	assert(h->heap);

	for(int i = 0; i < h->size; i++) h->heap[i] = 0;

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

	Heap j;
	j = h;

	printf("INSERT: %d, %s, %d ", key, r->room, r->temp);
	fflush(stdout); 

	int heapnum = j->n + 1;
	int maxheap = j->size;

	//demand(heapnum < maxheap, "heap overflow\n");
	while(heapnum  >= maxheap) {
		maxheap *= 2;
		printf("\nExpanding heap size to %d\n", maxheap);
		h->heap = realloc(h->heap, sizeof(struct heapslot *) * maxheap);
		demand(h != 0, "malloc fail in insert\n");
		printf("Realloc: %p\n", (void *) h->heap);
	}
	j->size = maxheap;

	heapslot *e;

    assert(r);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = key;
    e->value = r;
    //printRoom(value);

	cur = heapnum;

	j->heap[cur] = e;

	parent = cur/2;

	while (j->heap[parent] && j->heap[parent]->key > j->heap[cur]->key) {
		printf("\n--");
		printHeap(j);
		demand(parent > 0, "inserted item rising past root\n");
		swap(j->heap[parent], j->heap[cur]);
		cur = parent;
		parent = cur/2;
	}

	j->n++;

	printHeap(j);
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

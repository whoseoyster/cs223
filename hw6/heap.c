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

	for(int i = 0; i < h->size; i++)
	{
	    h->heap[i] = malloc(sizeof(*(h->heap[i])));
	    h->heap[i]->key = INT_MIN;
	    h->heap[i]->value = NULL;
  	}

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

	// Heap j;
	// j = h;

	if (debugflag) {
		printf("INSERT: %d, %s, %d ", key, r->room, r->temp);
		fflush(stdout);
	}

	// int heapnum = j->n + 1;
	h->n++;
	// int maxheap = h->size;

	//demand(heapnum < maxheap, "heap overflow\n");
	while(h->n  >= h->size) {
		h->size *= 2;
		printf("\nExpanding heap size to %d\n", h->size);
		h->heap = realloc(h->heap, sizeof(*(h->heap)) * h->size);
		demand(h != 0, "malloc fail in insert\n");
		printf("Realloc: %p\n", (void *) h->heap);
		for (int i = (h->size)/2; i < (h->size); i++)
	    {
	      printf("new Heapslot: %i\n", i);
	      h->heap[i] = malloc(sizeof(*(h->heap[i])));
	      h->heap[i]->key = INT_MIN;
	      h->heap[i]->value = NULL;
	    }
	}
	// j->size = maxheap;
	
    //printRoom(value);

	cur = (h->n);

	h->heap[cur]->key = key;
	h->heap[cur]->value = r;

	parent = cur/2;

	while (h->heap[parent] && h->heap[parent]->key > h->heap[cur]->key) {
		printf("\n--");
		printHeap(h);
		demand(parent > 0, "inserted item rising past root\n");
		swap(h->heap[parent], h->heap[cur]);
		cur = parent;
		parent = cur/2;
	}

	// j->n++;

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

    for(i = 0; i < h->size; i++) {
        e = h->heap[i];
        printf("Freeing: %d\n", e->key);
        if (e->value)
        	free(e->value);
        free(e);
    }

    free(h->heap);
    free(h);
}

// ///////////////////////////////////////////////////////////////////////////////
// // heap.c       erc39
// // Implements heap functions
// ///////////////////////////////////////////////////////////////////////////////

// #include <stdio.h>
// #include "heap.h"
// #include <stdbool.h>

// struct room {
//   char room[4];
//   int temp;
//   int ncount;
//   int visited;
//   char neighbors[10][4];
// };

// ///////////////////////////////////////////////////////////////////////////////
// // empty
// // Returns true if heap is empty
// ///////////////////////////////////////////////////////////////////////////////
// bool empty(Heap h)
// {
//   return h->n == 0;
// }

// ///////////////////////////////////////////////////////////////////////////////
// // demand
// // If the cond is false, print message and exit
// ///////////////////////////////////////////////////////////////////////////////
// void demand(int cond, char * msg)
// {
//   if (! cond)
//   {
//     fprintf(stderr, "%s\n", msg);
//     exit(1);
//   }
//   return;
// }

// ///////////////////////////////////////////////////////////////////////////////
// // printHeap
// // prints the heap
// ///////////////////////////////////////////////////////////////////////////////
// void printHeap(Heap h)
// {
//   printf("HEAP: ");
//   for (int i = 1; i <= h->n; i++)
//   {
//     printf("[%d: %s %d/%d] ", i, h->heap[i]->value->room, h->heap[i]->key,
//       h->heap[i]->value->temp);
//   }
//   printf("\n");
// }

// struct room * findmin(Heap h)
// {
//   demand(!empty(h), "findmin not allowed on empty heap\n");
//   return h->heap[1]->value;
// }

// ///////////////////////////////////////////////////////////////////////////////
// // initheap
// // Initializes a heap
// ///////////////////////////////////////////////////////////////////////////////
// Heap initheap()
// {
//   Heap h = malloc(sizeof(*h));
//   h->n = 0;
//   h->size = MAXHEAP;
//   h->heap = malloc(h->size * sizeof(*(h->heap))); // An array of heapslot pointers
//   for (int i = 0; i < h->size; i++)
//   {
//     h->heap[i] = malloc(sizeof(*(h->heap[i])));
//     h->heap[i]->key = INT_MIN;
//     h->heap[i]->value = NULL;
//   }
//   //h->heap[0]->key = INT_MIN;
//   return h;
// }

// ///////////////////////////////////////////////////////////////////////////////
// // swap
// // Searches a graph starting at 'key' for a room on fire by using a
// // depth-first search
// ///////////////////////////////////////////////////////////////////////////////
// void swap(heapslot * s1, heapslot * s2)
// {
//   if (debugflag)
//     printf("SWAP: %d <=> %d\n", s1->key, s2->key);
//   heapslot temp;
//   temp = *s1;
//   *s1 = *s2;
//   *s2 = temp;
// }

// ///////////////////////////////////////////////////////////////////////////////
// // insert
// // Inserts a room onto the heap
// ///////////////////////////////////////////////////////////////////////////////
// void insert(Heap h, int key, struct room * r)
// {
//   (h->n)++;
//   if (debugflag)
//     printf("INSERT: %d, %s, %d ", key, r->room, r->temp);
//   int cur, parent;
//   while(h->n >= h->size)
//   {
//     h->size *= 2;
//     h->heap = realloc(h->heap, sizeof(*(h->heap)) * h->size);
//     if (debugflag)
//     {
//       printf("\nExpanding heap size to %d\n", h->size);
//       printf("Realloc: %p\n", (void *)h->heap);
//     }
//     for (int i = (h->size)/2; i < h->size; i++)
//     {
//       h->heap[i] = malloc(sizeof(*(h->heap[i])));
//       h->heap[i]->key = INT_MIN;
//       h->heap[i]->value = NULL;
//     }
//     demand (h != 0, "malloc fail in insert\n");
//   }
//   cur = (h->n);
//   h->heap[cur]->key = key;
//   h->heap[cur]->value = r;
//   parent = cur/2;
//   while (h->heap[parent]->key > h->heap[cur]->key)
//   {
//     if (debugflag)
//     {
//       printf("\n--");
//       printHeap(h);
//     }
//     swap(h->heap[parent], h->heap[cur]);
//     cur = parent;
//     parent = cur/2;
//   }
//   if (debugflag)
//     printHeap(h);
// }

// ///////////////////////////////////////////////////////////////////////////////
// // deletemin
// // Deletes the minimum of the heap
// ///////////////////////////////////////////////////////////////////////////////
// void deletemin(Heap h)
// {
//   int cur, child;
//   demand(! empty(h), "deletemin not allowed on empty heap\n");
//   swap(h->heap[1], h->heap[h->n--]);
//   cur = 1;
//   child = 2;
//   while (child <= h->n)
//   {
//     if (debugflag)
//     {
//       printf("DELETEMIN:\n--");
//       printHeap(h);
//     }
//     if (child < h->n && h->heap[child+1]->key < h->heap[child]->key)
//       child++;
//     if (h->heap[cur]->key > h->heap[child]->key)
//     {
//       demand(child <= h->size, "falling past leaves\n");
//       swap(h->heap[cur], h->heap[child]);
//       cur = child;
//       child = 2*cur;
//     }
//     else
//       break;
//   }
//   if (debugflag)
//     printHeap(h);
// }

// ///////////////////////////////////////////////////////////////////////////////
// // destroyHeap
// // Returns true if heap is empty
// ///////////////////////////////////////////////////////////////////////////////
// void destroyHeap(Heap h)
// {
//   if (debugflag)
//     printf("DestroyHeap\n");
//   for (int i = 0; i < (h->size); i++)
//   {
//     if (i <= h->n)
//     {
//       if (debugflag)
//         printf("Freeing: %d\n", h->heap[i]->key);
//       if (h->heap[i]->value)
//       {
//         free(h->heap[i]->value);
//         h->heap[i]->value = NULL;//NEW
//       }
//     }
//     free(h->heap[i]);
//   }
//   free(h->heap);
//   free(h);
// }

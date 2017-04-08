#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "heap.h"
#include "dict.h"

void * mymalloc(size_t s, int i);

int main(int argc, char ** argv){

  debugflag = true;

  Heap h = initheap();

  struct room * r;

  //printHeap();

  if (argc > 1) {
    for (int i=1; i < argc; i++) {
      int val = atoi(argv[i]);
      if (val > 0) {
	r = mymalloc(sizeof(struct room *), 7);
	r->temp = val*val;
	sprintf(r->room, "R%d", val);
	insert(h, val, r);
      } else {
	deletemin(h);
      }
    }
  } else {
    fprintf(stderr, "Usage: heap [numbers]+\n");
  }
  destroyHeap(h);

}

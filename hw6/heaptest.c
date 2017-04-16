#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "/c/cs223/hw6/heap.h"
#include "/c/cs223/hw6/dict.h"

// findMin on empty heap

int main(void){

  
  // Initialize the heap
  Heap h = initheap();

  // Check empty
  if (empty(h)) {
    printf("Heap is empty\n");
  }

  deletemin(h);

  // Destroy the heap
  // destroyHeap *should* free not only the heap but also the rooms on the heap
  // you may assume that all room structs on the heap have been malloc'd.
  destroyHeap(h);

}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "/c/cs223/hw6/heap.h"
#include "/c/cs223/hw6/dict.h"

// Heap handles duplicates reasonably

int main(void){

  
  // Initialize the heap
  Heap h = initheap();

  // Check empty
  if (empty(h)) {
    printf("Heap is empty\n");
  }

  struct room * r;

  // Insert six rooms
  r = malloc(sizeof(struct room));
  sprintf(r->room, "R12");
  insert(h, 6, r);

  r = malloc(sizeof(struct room));
  sprintf(r->room, "R11");
  insert(h, 3, r);

  r = malloc(sizeof(struct room));
  sprintf(r->room, "R12");
  insert(h, 6, r);

  r = malloc(sizeof(struct room));
  sprintf(r->room, "R08");
  insert(h, 1, r);

  r = malloc(sizeof(struct room));
  sprintf(r->room, "R09");
  insert(h, 5, r);

  r = malloc(sizeof(struct room));
  sprintf(r->room, "R08");
  insert(h, 1, r);

  // Remove minimum
  // You can assume that any heaptest will:
      // 1. findMin
      // 2. free room returned by findMin
      // 3. deleteMin
  // All three steps will occur, in that order, in any heaptest
  // Thus, deleteMin is *not* responsible for freeing the room
  r = findmin(h);
  printf("room: %s\n", r->room);

  free(r);
  deletemin(h);

  // check new minimum
  r = findmin(h);
  printf("room: %s\n", r->room);

  // Check empty
  if (empty(h)) {
    printf("Heap is empty\n");
  }

  // Destroy the heap
  // destroyHeap *should* free not only the heap but also the rooms on the heap
  // you may assume that all room structs on the heap have been malloc'd.
  destroyHeap(h);

}

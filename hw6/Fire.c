/* 
 Fire.c

 CS 223b, hw6  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h6.v1.html

 Name: Rishab Ramanathan
 netid: rr566

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "dict.h"
#include "heap.h"

#define _GNU_SOURCE 1
#define FIRETEMP 400
#define dfsbool 5 // If r->visited == 5, dfs has visited this room
#define bfsbool 6 // These constants enable me to run consecutive searches
#define bestbool 7 // on the graph without having to manually reset each
#define connbool 8 // 'visited' parameter
#define dirbool 9

char *strdup(const char *s);

#define ungetchar(c)  ungetc(c, stdin)    // unread char read from stdin

// Function Prototypes
int dfsearch (Dict d, char * key);
int bfsearch (Dict d, Heap queue, char * key);
// int bestsearch(Dict d, Heap h, char * room);
// int connectivity (Dict d, char * key, int * a);
// int directed (Dict d);
// int hasPath(Dict d, char * start, char * end);
void exitandfree(Dict d, Heap queue, Heap h);

int main(int argc, char **argv) {

  // bool room_bool = false;
  bool dfs = false;
  bool bfs = false;
  bool conn = false;
  bool dir = false;
  bool best = false;

  char room[4] = "una";

  if (argc == 1) {
    fprintf(stderr, "Fatal error: no room given.\n" );
    printf("Usage: Fire -room value [-dfs | -bfs | -best | -conn | -dir]\n");
    exit(1);
  }

  for (int l = 1; l < argc; l++) {
    if (strcmp(argv[l], "-room") == 0) {
      l++;
      if (l >= argc) {
        fprintf(stderr, "Fatal error: no room given.\n" );
        printf("Usage: Fire -room value [-dfs | -bfs | -best | -conn | -dir]\n");
        exit(1);
      }
      strcpy(room, argv[l]);
    } else if (strcmp(argv[l], "-dfs") == 0) {
      dfs = true;
    } else if (strcmp(argv[l], "-bfs") == 0) {
      bfs = true;
    } else if (strcmp(argv[l], "-conn") == 0) {
      conn = true;
    } else if (strcmp(argv[l], "-dir") == 0) {
      dir = true;
    } else if (strcmp(argv[l], "-best") == 0) {
      best = true;
    } else if (strcmp(argv[l], "-debug") == 0) {
      debugflag = true;
    } else {
      fprintf(stderr, "Fatal error: invalid command line argument: %s\n", argv[l]);
      exit(1);
    }
  }

  //declare input processing variables
  char in[1030];
  char c;
  bool _d = false;
  int i=0;
  int j=0;
  int k=0;
  bool kill = false;
  bool skip=false;

  char *tok;

  struct room * r;

  Dict d = DictCreate();
  Heap queue = initheap(); // heap for bfs
  Heap h = initheap(); // heap for best search

  memset(in, '\0', 1030);

  if (!feof(stdin)) {
    kill=true;
  }

  while (true) {
    while((c = getchar()) != '\n' && c != EOF) {
      in[i] = c;
      i++;
      _d = true;
    }

    if (_d) {
      _d = false;
      skip = false;
      i=0;
      j=0;
      tok = strtok(in, " ");

      r = malloc(sizeof(struct room *) * 7);
      assert(r);
      r->visited = 0;

      for (int k=0; k<10;k++) {
        memset(r->neighbors[k], '\0', 4);
      }
      
      while( tok != NULL ) 
      {
        // int len = strlen(tok);
        skip = false;
        if (j == 0) {
          sprintf(r->room, "%s", tok);
        } else if (j == 1) {
          r->temp = atoi(tok);
        } else {
          if (strcmp(r->room, tok) == 0) {
            // printf("self-reference: %s\n", tok);
            tok = strtok(NULL, " ");
            continue;
          }
          for (i=0; i<j-2; i++) {
            if (strcmp(r->neighbors[i], tok) == 0) {
              // printf("skipping duplicate neighbor %s\n", tok);
              skip = true;
              j--;
              break;
            }
          }
          if (!skip)
            sprintf(r->neighbors[j-2], "%s", tok);
        }
        j++;
        tok = strtok(NULL, " ");
      }
      r->ncount = j-2;

      if (DictSearch(d, r->room)) {
        DictDelete(d, r->room);
        fprintf(stderr, "Room %s already in graph.  Replacing it.\n", r->room);
        k--;
      }
      DictInsert(d, r->room, r);

      k++;
      i=0;

      memset(in, '\0', 1030);
    }
      if (kill) {
        if (feof(stdin)) break;
      }
    }

    struct elt *e;              /* what to print */

    for(k=0; k<d->size; k++) {
      if (d->table[k]) {
        for (e = d->table[k];e != NULL; e = e->next){
          r = e->value;
          for (i=0; i<10; i++) {
            if (r->neighbors[i][0] != '\0') {
              if (DictSearch(d, r->neighbors[i]) == 0) {
                fprintf(stderr, "Warning: room %s has neighbor %s which is not in the dictionary.\n", r->room, r->neighbors[i]);
              }
            }
          }
        }
      }
    }

  if (dfs || (!dfs && !bfs && !best && !conn && !dir))
  {
      printf("Starting depth first search:");
      if (!dfsearch (d, room))
        printf("  FAILED\n");
  }

  if (bfs)
  {
    printf("Starting breadth first search:");
    if (!bfsearch (d, queue, room))
      printf("  FAILED\n");
  }
  // if (best)
  // {
  //   printf("Starting best first search:");
  //   if (!bestsearch (graph, h, room))
  //     printf("  FAILED\n");
  // }
  // if (conn)
  // {
  //   int a = 0;
  //   connectivity (graph, room, &a);
  //   if (a == graph->n)
  //     printf("Graph is connected.\n");
  //   else
  //     printf("Graph is NOT connected.\n");
  // }
  // if (dir)
  // {
  //   directed(graph);
  // }

  exitandfree(d, queue, h);

  return 0;

}

///////////////////////////////////////////////////////////////////////////////
// dfsearch
// Searches a graph starting at 'key' for a room on fire by using a
// depth-first search
///////////////////////////////////////////////////////////////////////////////
int dfsearch (Dict d, char * key)
{
  struct room * r = DictSearch(d, key);
  if (!r)
    return 0;
  if (r->visited == dfsbool)
    return 0;
  printf(" %s", r->room);
  if (r->temp > FIRETEMP)
  {
    printf("  SUCCESS!\n");
    return 1;
  }
  else
  {
    r->visited = dfsbool;
    for (int i = 0; i < r->ncount; i++)
    {
      if (dfsearch (d, r->neighbors[i]))
        return 1;
    }
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// bfsearch
// Searches a graph starting at 'key' for the room on fire by using a
// breadth-first search
///////////////////////////////////////////////////////////////////////////////
int bfsearch (Dict d, Heap queue, char * key)
{
  int num = 0;
  struct room * r = DictSearch(d, key);
  if (r)
  {
    r->visited = bfsbool;
    insert(queue, num++, r);
  }
  while (!empty(queue))
  {
    r = findmin(queue);
    printf( " %s", r->room);
    if (r->temp > FIRETEMP)
    {
      printf("  SUCCESS!\n");
      return 1;
    }
    for (int i = 0; i < r->ncount; i++)
    {
      struct room * neighbor = DictSearch(d, r->neighbors[i]);
      if (neighbor && (neighbor->visited) != bfsbool)
      {
        neighbor->visited = bfsbool;
        insert(queue, num++, neighbor);
      }
    }
    deletemin(queue);
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// exitandfree
// Exits and frees remaining memory
///////////////////////////////////////////////////////////////////////////////
void exitandfree(Dict d, Heap queue, Heap h)
{
  //destroyHeap(queue);
  //destroyHeap(h);
  DictDestroy(d);
  //exit(0);

  for (int i = 0; i < queue->size; i++)
  {
    printf("nana\n");
    free(queue->heap[i]);
  }
  printf("akaka\n");
  free (queue->heap);
  printf("babab\n");
  free (queue);

  for (int i = 0; i < h->size; i++)
  {
    free(h->heap[i]);
  }
  free (h->heap);
  free (h);
  exit(0);
}

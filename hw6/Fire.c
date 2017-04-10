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

char *strdup(const char *s);

#define ungetchar(c)  ungetc(c, stdin)    // unread char read from stdin

int main(int argc, char **argv) {

  // bool room_bool = false;
  bool dfs = false;
  bool bfs = false;
  bool con = false;
  bool dir = false;

  char * room;

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
      room = argv[l];
    } else if (strcmp(argv[l], "-dfs") == 0) {
      dfs = true;
    } else if (strcmp(argv[l], "-bfs") == 0) {
      bfs = true;
    } else if (strcmp(argv[l], "-conn") == 0) {
      con = true;
    } else if (strcmp(argv[l], "-dir") == 0) {
      dir = true;
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
  bool kill = false;

  char *tok;

  struct room * r;

  Dict d = DictCreate();

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
      i=0;
      j=0;
      tok = strtok(in, " ");

      r = malloc(sizeof(struct room *));
      assert(r);

      while( tok != NULL ) 
      {
        // int len = strlen(tok);

        if (j == 0) {
          sprintf(r->room, "%s", tok);
        } else if (j == 1) {
          r->temp = atoi(tok);
        } else {
          sprintf(r->neighbors[j-2], "%s", tok);
        }
        j++;
        tok = strtok(NULL, " ");
      }
      r->ncount = j-2;

      DictInsert(d, strdup(r->room), r);

      DictPrint(d);

      i=0;
      memset(in, '\0', 1030);
    }
      if (kill) {
        if (feof(stdin)) break;
      }
    }

    DictPrint(d);

  // for (i=0; i<)
  //   DictSearch(d, )

  return 0;

}

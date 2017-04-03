/* 
 Cloud.c

 CS 223b, hw5  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h5.v1.html

 Name: Rishab Ramanathan
 netid: rr566

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "/c/cs223/hw5/btree.h"
#include "avltree.h"

#define _GNU_SOURCE 1

int main(int argc, char **argv) {
  //debug statement
  bool debugflag = false;
  int threshold = 5;
  bool post = false;
  bool ino = false;
  bool pre = false;
  bool html = false;
  bool avl = false;

  for (int l = 1; l < argc; l++) {
    if (strcmp(argv[l], "-debug") == 0) {
      debugflag = true;
    } else if (strcmp(argv[l], "-threshold") == 0) {
      l++;
      if (l >= argc) {
        fprintf(stderr, "Missing threshold argument on command line.\n");
      }
      int v = atoi(argv[l]);
      if (v == 0 || v == 1) {
        fprintf(stderr, "Invalid threshold value: %s\n", argv[l]);
        continue;
      }
      threshold = v;
    } else if (strcmp(argv[l], "-preorder") == 0) {
      pre = true;
    } else if (strcmp(argv[l], "-inorder") == 0) {
      ino = true;
    } else if (strcmp(argv[l], "-postorder") == 0) {
      post = true;
    } else if (strcmp(argv[l], "-html") == 0) {
      html = true;
    } else if (strcmp(argv[l], "-avl") == 0) {
      avl = true;
    } else {
      fprintf(stderr, "Fatal error: invalid command line argument: %s\n", argv[l]);
      exit(1);
    }
  }

  //declare input processing variables
  char in[1030];
  char c;
  bool d = false;
  int i=0;
  bool kill = false;

  char *tok;

  struct node * root = 0;
  if (!avl) 
    root = makeNode("**root**");
  struct node * cloud = 0;
  struct node * oldnode = 0;
  struct tree * aroot = TREE_EMPTY;
  struct tree * acloud = 0;
  struct tree * aoldnode = 0;

  memset(in, '\0', 1030);

  if (!feof(stdin)) {
    kill=true;
    if (getchar() == EOF) {
      printf("Input: \n");
    } else {
      ungetchar();
    }
  }

  while (true) {
    while((c = getchar()) != '\n' && c != EOF) {
      in[i] = c;
      i++;
      d = true;
    }

    if (d) {
      d = false;
      i=0;

      if (debugflag)
        printf("Input: %s\n", in);

      tok = strtok(in, " ");

      while( tok != NULL ) 
      {
        int len = strlen(tok);
        bool skip = false;

        for (int j=0; j<len;j++) {
          if (!isalpha(tok[j])) {
            skip = true;
            tok = strtok(NULL, " ");
            break;
          }
          tok[j] = tolower(tok[j]);
        }
        if (skip) continue;

        if (avl) {
          aoldnode = treeContains(aroot, tok);
          if (aoldnode) {
            aoldnode->count++;
            if (aoldnode->count == threshold) {
              aoldnode->next = acloud;
              acloud = aoldnode;
            }
          } else {
            atreeInsert(&aroot, tok);
          }
        } else {
          oldnode = treeSearch(root, tok);
          if (oldnode) {
            oldnode->count++;
            if (oldnode->count == threshold) {
              oldnode->next = cloud;
              cloud = oldnode;
            }
          } else {
            treeInsert(root, tok);
          }
        }

        tok = strtok(NULL, " ");
      }

      i=0;
      memset(in, '\0', 1030);
    }
      if (kill) {
        if (feof(stdin)) break;
      }
    }

  if (avl) {
    if (debugflag) {
      printf("Tree height: %i\n", (atreeHeight(aroot)));
      printf("Tree size: %zu\n", (atreeSize(aroot)));
      treePrint(aroot);
    }

    if (pre) {
      printf("PREORDER\n");
      aprintTreePre(aroot);
      putchar('\n');
    }
    if (ino) {
      printf("INORDER\n");
      aprintTreeIn(aroot);
      putchar('\n');
    }
    if (post) {
      printf("POSTORDER\n");
      aprintTreePost(aroot);
      putchar('\n');
    }

    if (acloud == 0) {
      printf("No words seen %i times.\n", threshold);
    } else {
      if (!html) 
        printf("The Word Cloud:\n");
      for (aoldnode = acloud; aoldnode != 0; aoldnode = aoldnode->next) {
        if (html) {
          printf("<span style=\"font-size: %ipx\"> %s </span>\n", aoldnode->count, aoldnode->key);
        } else {
          printf("[%i] %s [%i]\n", i, aoldnode->key, aoldnode->count);
        }
        i++;
      }
    }
    atreeDestroy(&aroot);
  } else {
    if (debugflag) {
      printf("Tree height: %i\n", (treeHeight(root)));
      printf("Tree size: %i\n", (treeSize(root)));
    }

    if (pre) {
      printf("PREORDER\n");
      printTreePre(root);
      putchar('\n');
    }
    if (ino) {
      printf("INORDER\n");
      printTreeIn(root);
      putchar('\n');
    }
    if (post) {
      printf("POSTORDER\n");
      printTreePost(root);
      putchar('\n');
    }

    if (cloud == 0) {
      printf("No words seen %i times.\n", threshold);
    } else {
      if (!html) 
        printf("The Word Cloud:\n");
      for (oldnode = cloud; oldnode != 0; oldnode = oldnode->next) {
        if (html) {
          printf("<span style=\"font-size: %ipx\"> %s </span>\n", oldnode->count, oldnode->key);
        } else {
          printf("[%i] %s [%i]\n", i, oldnode->key, oldnode->count);
        }
        i++;
      }
    }
    treeDestroy(root);
  }

  return 0;

}

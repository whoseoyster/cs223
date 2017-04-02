/* 
 Cloud.c

 CS 223b, hw5  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h5.v1.html

 Name: Rishab Ramanathan
 netid: rr566

 */

// #define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "/c/cs223/hw5/btree.h"
// #include "avltree.h"

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
  int z=0;
  bool kill = false;

  char *tok;

  memset(in, '\0', 1030);

  if (!feof(stdin)) kill=true;

  while (true) {
    while((c = getchar()) != EOF) {
      if (c != '\n')
        in[i] = c;
      
      i++;
      if (debugflag && c == '\n') {
        printf("Input: %s\n", in + z);
        z = i;
      }
      d = true;
    }

    if (d) {
      d = false;
      i=0;

      printf("boooo %s\n", in);

      tok = strtok(in, " ");

      struct node * root = makeNode("**root**");
      struct node * cloud = 0;
      struct node * oldnode = 0;
      // struct tree *root = TREE_EMPTY;

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

        tok = strtok(NULL, " ");
      }

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

      i=0;
      memset(in, '\0', 1030);
      treeDestroy(root);
    }
    if (kill) {
      if (feof(stdin)) break;
    }
  }

  return 0;

}

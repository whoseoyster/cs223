/*
 * Basic binary search tree data structure without balancing info.
 *
 * Convention: 
 *
 * Operations that update a tree are passed a struct tree **,
 * so they can replace the argument with the return value.
 *
 * Operations that do not update the tree get a const struct tree *.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#define LEFT (0)
#define RIGHT (1)
#define TREE_NUM_CHILDREN (2)

struct tree {
    /* we'll make this an array so that we can make some operations symmetric */
    struct tree *child[TREE_NUM_CHILDREN];
    char * key;
    int count;
    int height;    /* height of this node */
    size_t size;   /* size of subtree rooted at this node */
    struct tree *next; /* for cloud */
};

#define TREE_EMPTY (0)
#define TREE_EMPTY_HEIGHT (-1)

/* free all elements of a tree, replacing it with TREE_EMPTY */
void atreeDestroy(struct tree **root);

/* insert an element into a tree pointed to by root */
void atreeInsert(struct tree **root, char * newElement);

/* return 1 if target is in tree, 0 otherwise */
/* we allow root to be modified to allow for self-balancing trees */
struct tree * treeContains(struct tree *root, char * target);

/* delete minimum element from the tree and return its key */
/* do not call this on an empty tree */
char * treeDeleteMin(struct tree **root);

/* delete target from the tree */
/* has no effect if target is not in tree */
void treeDelete(struct tree **root, char * target);

/* return height of tree */
int atreeHeight(const struct tree *root);

/* return size of tree */
size_t atreeSize(const struct tree *root);

/* pretty-print the contents of a tree */
void treePrint(const struct tree *root);

/* return the number of elements in tree less than target */
size_t treeRank(const struct tree *root, char * target);

/* return an element with the given rank */
/* rank must be less than treeSize(root) */
char * treeUnrank(const struct tree *root, size_t rank);

// print the contents of the tree in preorder, inorder, and postorder,
// respectively.
void aprintTreePre(struct tree * t);
void aprintTreeIn(struct tree * t);
void aprintTreePost(struct tree * t);

#include "avltree.h"

#define _GNU_SOURCE 1

int
atreeHeight(const struct tree *root)
{
    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        return root->height;
    }
}

/* recompute height from height of kids */
static int
treeComputeHeight(const struct tree *root)
{
    int childHeight;
    int maxChildHeight;
    int i;

    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        maxChildHeight = TREE_EMPTY_HEIGHT;

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            childHeight = atreeHeight(root->child[i]);
            if(childHeight > maxChildHeight) {
                maxChildHeight = childHeight;
            }
        }

        return maxChildHeight + 1;
    }
}

size_t
atreeSize(const struct tree *root)
{
    if(root == 0) {
        return 0;
    } else {
        return root->size;
    }
} 

/* recompute size from size of kids */
static int
treeComputeSize(const struct tree *root)
{
    int size;
    int i;

    if(root == 0) {
        return 0;
    } else {
        size = 1;

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            size += atreeSize(root->child[i]);
        }

        return size;
    }
}

/* fix aggregate data in root */
/* assumes children are correct */
static void
treeFix(struct tree *root)
{
    if(root) {
        root->height = treeComputeHeight(root);
        root->size = treeComputeSize(root);
    }
}

/* rotate child in given direction to root */
static void
treeRotate(struct tree **root, int direction)
{
    struct tree *x;
    struct tree *y;
    struct tree *b;

    /*
     *      y           x 
     *     / \         / \
     *    x   C  <=>  A   y
     *   / \             / \
     *  A   B           B   C
     */

    y = *root;                  assert(y);
    x = y->child[direction];    assert(x);
    b = x->child[!direction];

    /* do the rotation */
    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;

    /* fix y then x */
    treeFix(y);
    treeFix(x);
}

/* restore AVL property at *root after an insertion or deletion */
static void
treeRebalance(struct tree **root)
{
    int tallerKid;

    if(*root) {
        for(tallerKid = 0; tallerKid < TREE_NUM_CHILDREN; tallerKid++) {
            if(atreeHeight((*root)->child[tallerKid]) >= atreeHeight((*root)->child[!tallerKid]) + 2) {

                /* check if zig-zag: opposite-direction nephew is the tall one */
                /* this also covers case where both nephews are too tall */
                if(atreeHeight((*root)->child[tallerKid]->child[!tallerKid]) 
                            >= atreeHeight((*root)->child[tallerKid]) - 1) {
                    /* zig zag case */
                    treeRotate(&(*root)->child[tallerKid], !tallerKid);
                }

                /* fall through to zig zig case */
                treeRotate(root, tallerKid);

                /* don't bother with other kid */
                break;
            }
        }
    }
}


/* free all elements of a tree, replacing it with TREE_EMPTY */
void 
atreeDestroy(struct tree **root)
{
    int i;

    if(*root) {
        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            atreeDestroy(&(*root)->child[i]);
        }
        free((*root)->key);
        free(*root);
        *root = 0;
    }
}

/* return 1 if target is in tree, 0 otherwise */
struct tree *
treeContains(struct tree *t, char * target)
{
    while(t && strcmp(t->key, target) != 0) {
        int chld = 0;
        if (strcmp(t->key, target) < 0) {
            chld = 1;
        }
        t = t->child[chld];
    }
    return t;
}

/* insert an element into a tree pointed to by root */
void
atreeInsert(struct tree **root, char * newElement)
{
    struct tree *e;

    if(*root == 0) {
        /* not already there, put it in */

        e = malloc(sizeof(*e));
        assert(e);

        e->key = strdup(newElement);
        e->count = 1;
        e->child[LEFT] = e->child[RIGHT] = 0;

        *root = e;
    } else {
        /* do this recursively so we can fix data on the way back out */
        if (strcmp((*root)->key, newElement) < 0) {
            atreeInsert(&(*root)->child[1], newElement);
        } else if (strcmp((*root)->key, newElement) == 0) {
            (*root)->count++;
        } else {
            atreeInsert(&(*root)->child[0], newElement);
        }
    }

    /* fix the aggregate data */
    treeFix(*root);
    treeRebalance(root);
}

/* delete minimum element from the tree and return its key */
/* do not call this on an empty tree */
char *
treeDeleteMin(struct tree **root)
{
    struct tree *toFree;
    char * retval;

    assert(*root);  /* can't delete min from empty tree */

    if((*root)->child[LEFT]) {
        /* recurse on left subtree */
        retval = treeDeleteMin(&(*root)->child[LEFT]);
    } else {
        /* delete the root */
        toFree = *root;
        retval = toFree->key;
        *root = toFree->child[RIGHT];
        free(toFree);
    }

    /* fix the aggregate data */
    treeFix(*root);
    treeRebalance(root);

    return retval;
}

/* delete target from the tree */
/* has no effect if target is not in tree */
void 
treeDelete(struct tree **root, char * target)
{
    struct tree *toFree;

    /* do nothing if target not in tree */
    if(*root) {
        if((*root)->key == target) {
            if((*root)->child[RIGHT]) {
                /* replace root with min value in right subtree */
                (*root)->key = treeDeleteMin(&(*root)->child[RIGHT]);
            } else {
                /* patch out root */
                toFree = *root;
                *root = toFree->child[LEFT];
                free(toFree);
            }
        } else {
            if (strcmp((*root)->key, target) > 0) {
                treeDelete(&(*root)->child[0], target);
            } else if (strcmp((*root)->key, target) == 0) {
                treeDelete(&(*root)->child[1], target);
            } else {
                treeDelete(&(*root)->child[1], target);
            }
            
        }

        /* fix the aggregate data */
        treeFix(*root);
        treeRebalance(root);
    }
}


/* how far to indent each level of the tree */
#define INDENTATION_LEVEL (2)

/* print contents of a tree, indented by depth */
static void
treePrintIndented(const struct tree *root, int depth)
{
    int i;

    if(root != 0) {
        treePrintIndented(root->child[LEFT], depth+1);

        for(i = 0; i < INDENTATION_LEVEL*depth; i++) {
            putchar(' ');
        }
        printf("%s Height: %d Size: %zu Count: %d (%p)\n", root->key, root->height, root->size, root->count, (void *) root);

        treePrintIndented(root->child[RIGHT], depth+1);
    }
}

/* print the contents of a tree */
void 
treePrint(const struct tree *root)
{
    treePrintIndented(root, 0);
}

size_t
treeRank(const struct tree *t, char * target)
{
    size_t rank = 0;

    while(t && strcmp(t->key, target) != 0) {

        if(strcmp(t->key, target) < 0) {
            /* go right */
            /* root and left subtree are all less than target */
            rank += (1 + atreeSize(t->child[LEFT]));
            t = t->child[RIGHT];
        } else {
            /* go left */
            t = t->child[LEFT];
        }
    }

    /* we must also count left subtree */
    return rank + atreeSize(t->child[LEFT]);
}

char *
treeUnrank(const struct tree *t, size_t rank)
{
    size_t leftSize;

    /* basic idea: if rank < atreeSize(child[LEFT]), recurse in left child */
    /* if it's equal, return the root */
    /* else recurse in right child with rank = rank - atreeSize(child[LEFT]) - 1 */
    while(rank != (leftSize = atreeSize(t->child[LEFT]))) {
        if(rank < leftSize) {
            t = t->child[LEFT];
        } else {
            t = t->child[RIGHT];
            rank -= (leftSize + 1);
        }
    }

    return t->key;
}

// print the contents of the tree in preorder, inorder, and postorder,
// respectively.
void 
aprintTreePre(struct tree * t) 
{
    printf("%s [%i / %i] ", t->key, t->count, atreeHeight(t));
    if (t->child[LEFT] != 0) {
        aprintTreePre(t->child[LEFT]);
    }
    if (t->child[RIGHT] != 0) {
        aprintTreePre(t->child[RIGHT]);
    }
}

void 
aprintTreeIn(struct tree * t)
{
    if (t->child[LEFT] != 0) {
        aprintTreeIn(t->child[LEFT]);
    }
    printf("%s [%i / %i] ", t->key, t->count, atreeHeight(t));
    if (t->child[RIGHT] != 0) {
        aprintTreeIn(t->child[RIGHT]);
    }
}
void 
aprintTreePost(struct tree * t)
{
    if (t->child[LEFT] != 0) {
        aprintTreePost(t->child[LEFT]);
    }
    if (t->child[RIGHT] != 0) {
        aprintTreePost(t->child[RIGHT]);
    }
    printf("%s [%i / %i] ", t->key, t->count, atreeHeight(t));
}

// #ifdef TEST_MAIN


// int
// main(int argc, char **argv)
// {
//     // int i;
//     // const int n = 10;
//     struct tree *root = TREE_EMPTY;

//     if(argc != 1) {
//         fprintf(stderr, "Usage: %s\n", argv[0]);
//         return 1;
//     }

//     // char test[10] = {'a', 'c', 'a', 'a', 'a', 'a', 'b', 'b', 'b', 'c'};

// //     for(i = 0; i < 10; i++) {
// //         assert(!treeContains(root, test[i]));
// //         atreeInsert(&root, test[i]);
// //         assert(treeContains(root, test[i]));
// // #ifdef PRINT_AFTER_OPERATIONS
// //         treePrint(root);
// //         puts("---");
// // #endif
// //     }

//     // assert(!treeContains(root, "a"));
//     atreeInsert(&root, "a");
//     // assert(treeContains(root, "a"));
//     treePrint(root);
//     puts("---");

//     // printf("bitch: %s\n", (treeContains(root, "a"))->key);
//     treeContains(root, "a");

//     printf("Tree height: %i\n", atreeHeight(root));
//     printf("Tree size: %zu\n", atreeSize(root));

//     aprintTreePre(root);
//     putchar('\n');
//     aprintTreeIn(root);
//     putchar('\n');
//     aprintTreePost(root);
//     putchar('\n');

//     // /* check ranks */
//     // for(i = 0; i < n; i++) {
//     //     assert(treeRank(root, i) == i);
//     //     assert(treeUnrank(root, i) == i);
//     // }

//     // treeSanityCheck(root);

// //     /* now delete everything */
// //     for(i = 0; i < n; i++) {
// //         assert(treeContains(root, i));
// //         treeDelete(&root, i);
// //         assert(!treeContains(root, i));
// // #ifdef PRINT_AFTER_OPERATIONS
// //         treePrint(root);
// //         puts("---");
// // #endif
// //     }

//     // treeSanityCheck(root);

//     atreeDestroy(&root);

//     return 0;
// }

// #endif

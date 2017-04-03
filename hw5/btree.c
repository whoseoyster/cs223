#include "/c/cs223/hw5/btree.h"

#define _GNU_SOURCE 1

char *strdup(const char *s);

// create and initialize a node
struct node *makeNode(char * key) {
  struct node *a;

  a = malloc(sizeof(*a));
  assert(a);

  a->key = strdup(key);
  a->count = 1;
  a->left = 0;
  a->right = 0;
  a->next = 0;
  a->height = 0;
  a->parent = 0;

  return a; 
}

// see Aspnes
int
treeSize(struct node *root)
{
    if(root == 0) {
        return 0;
    } else {
        return 1 + treeSize(root->left) + treeSize(root->right);
    }
}

int
treeHeight(struct node *root)
{
    int lh;     /* height of left subtree */
    int rh;     /* height of right subtree */

    if(root == 0) {
        return -1;
    } else {
        lh = treeHeight(root->left);
        rh = treeHeight(root->right);
        return 1 + (lh > rh ? lh : rh);
    }
}

/* returns node with given target key */
/* or null if no such node exists */
struct node * 
treeSearch(struct node *root, char * target)
{
	struct node *lh;
	struct node *rh;

	if (root == 0) {
		return NULL;
	} else if (strcmp(root->key, target) == 0) {
		return root;
	} else {
		if ((lh = treeSearch(root->left, target)) != NULL) {
			return lh;
		} else if ((rh = treeSearch(root->right, target)) != NULL){
			return rh;
		} else {
			return NULL;
		}
	}
}

// adds a node with given key to tree
void 
treeInsert(struct node *root, char * key) 
{
	if (strcmp(key, root->key) == 0) {
		root->count++;
	} else if (strcmp(key, root->key) < 0) {
		if (root->left == 0) {
			struct node *new = makeNode(key);
			root->left = new;
			new->parent = root;
		} else {
			treeInsert(root->left, key);
		}
	} else if (strcmp(key, root->key) > 0) {
		if (root->right == 0) {
			struct node *new = makeNode(key);
			root->right = new;
			new->parent = root;
		} else {
			treeInsert(root->right, key);
		}
	}
}

// print the contents of the tree in preorder, inorder, and postorder,
// respectively.
void 
printTreePre(struct node * tree) 
{
	printf("%s [%i / %i] ", tree->key, tree->count, treeHeight(tree));
	if (tree->left != 0) {
		printTreePre(tree->left);
	}
	if (tree->right != 0) {
		printTreePre(tree->right);
	}
}

void 
printTreeIn(struct node * tree)
{
	if (tree->left != 0) {
		printTreeIn(tree->left);
	}
	printf("%s [%i / %i] ", tree->key, tree->count, treeHeight(tree));
	if (tree->right != 0) {
		printTreeIn(tree->right);
	}
}
void 
printTreePost(struct node * tree)
{
	if (tree->left != 0) {
		printTreePost(tree->left);
	}
	if (tree->right != 0) {
		printTreePost(tree->right);
	}
	printf("%s [%i / %i] ", tree->key, tree->count, treeHeight(tree));
}

// free up the space allocated for the tree
void treeDestroy(struct node * tree)
{
	if (tree->left != 0) {
		treeDestroy(tree->left);
	}
	if (tree->right != 0) {
		treeDestroy(tree->right);
	}
	free(tree->key);
	free(tree);
}


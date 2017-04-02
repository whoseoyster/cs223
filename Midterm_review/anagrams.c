#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

int cmpfunc(const void *p1, const void *p2) {

  return *(char*)p1 - *(char*)p2;

}

// bool anagram(char *s, char *s2)   {
//     int i = 0;
//     int j = 0;
//     int len = strlen(s);
     
//     for(i = len; i> 0; i--)   {
//     	if (s[i-1] == s2[len-i]) {
//     		continue;
//     	}
//     	printf("false\n");
//     	return false;
//     }
//     printf("true\n");
//     return true;
// }

struct node {
  char * key;
  int count;
  struct node *left;  /* left child */
  struct node *right; /* right child */
  struct node *next;  /* link to other nodes over threshold for Cloud */
  // option for more efficient height calculation
  int height;
  struct node *parent;  
};

// create and initialize a node
struct node *makeNode(char * key) {
  struct node *a;

  a->key = key;
  a->count = 0;
  a->left = 0;
  a->right = 0;
  a->next = 0;
  a->height = 0;
  a->parent = 0;

  return a;
}

int main(int argc, char ** argv) {
  
  // char word[4] = "rut";
  // char word2[4] = "urr";
  
  // qsort(word, strlen(word), sizeof(word[0]), cmpfunc);
  // qsort(word2, strlen(word2), sizeof(word[0]), cmpfunc);

  // if (strcmp(word, word2) == 0) {
  //   printf("yes\n");
  // } else {
  //   printf("no\n");
  // }

  char *foo;
  char *bar;
  int res;

  foo = "abc";
  bar = "abd";

  struct node *blah = makeNode(foo);

  printf("%s\n", blah->key);

  return 0;
}

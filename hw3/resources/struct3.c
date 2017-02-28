#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv){

  struct node{
    long a, b, c, d, e;
  };

  struct node *ptr = NULL;
  struct node *ptr2 = NULL;
  
  ptr = (struct node*)malloc(sizeof (ptr));  // Your Line 1
  ptr2 = malloc(sizeof(struct node));        // alloc another struct on the heap

  printf("sizeof(ptr): %d and sizeof(struct node) = %d\n", sizeof(ptr), sizeof(struct node));
  printf("ptr: %p and ptr2: %p\n", ptr, ptr2);
 
  ptr2->a = 11; ptr2->b = 12; ptr2->c = 13; ptr2->d = 14; ptr2->e = 15;
  ptr->a = 1; ptr->b = 2; ptr->c = 3; ptr->d = 4; ptr->e = 5;

  //ptr2->a = 11; ptr->b = 12; ptr->c = 13; ptr->d = 14; ptr->e = 15;

  printf("sizeof(int): %d  sizeof(long): %d sizeof(ptr): %d\n",
	 sizeof(int), sizeof(long), sizeof(ptr));
  printf("ptr:  a: %d, b: %d, c: %d, d: %d, e: %d\n",
	 ptr->a, ptr->b, ptr->c, ptr->d, ptr->e);
  printf("ptr2: a: %d, b: %d, c: %d, d: %d, e: %d\n",
	 ptr2->a, ptr2->b, ptr2->c, ptr2->d, ptr2->e);

  free(ptr);
  free(ptr2);
}
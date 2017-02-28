#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct elt {
    struct elt *next;
    int value;
};

/* 
 * We could make a struct for this,
 * but it would have only one component,
 * so this is quicker.
 */
typedef struct elt *List;


/* insert a new value on the list */
void insert(List *lst, int value)
{
    struct elt *e;

    e = malloc(sizeof(struct elt));
    assert(e);

    e->value = value;
    e->next = *lst;
    *lst = e;
}

int listEmpty(const List *lst)
{
    return (*lst == 0);
}

int pop(List *lst)
{
    int ret;
    struct elt *e;

    assert(!listEmpty(lst));

    ret = (*lst)->value;

    /* patch out first element */
    e = *lst;
    *lst = e->next;

    free(e);

    return ret;
}

/* print contents of list on a single line */
void listPrint(const List *lst)
{
    struct elt *e;

    for(e = *lst; e != 0; e = e->next) {
        printf("%d ", e->value);
    }
    
    putchar('\n');
}

/* mystery function */
void mystery(const List *lst)
{
  List e = *lst;
  if (!e) return;
  mystery(&e->next);
  printf("%d ", e->value);
  return;
}


/* find maximum value */
int max(const List *lst)
{
  struct elt *e;

  if (listEmpty(lst)) {
    fprintf(stderr, "Warning: no max in empty list.\n");
  }
  int max = (*lst)->value;
  for(e = (*lst)->next; e != 0; e = e->next) {
    if (e->value > max) {
      max = e->value;
    }
  }
  
  return max;
}

/* find length of list */
int length(const List *lst)
{
    struct elt *e;

    int len = 0;
    for(e = *lst; e != 0; e = e->next) {
      len++;
    }

    return len;
}

/* find nth node of list - zero-based */
List nth(const List *lst, int n)
{
    struct elt *e;

    int count = 0;
    for(e = *lst; e != 0; e = e->next) {
      if (count == n) {
	return e;
      }
      count++;
    }
    printf("Fewer than %d elements in list.\n", n);
    return NULL;
}

/* find nth node from back of list - zero-based */

/*
Note: this was a sample midterm question

Algorithm:

1. Create two pointers, p1 and p2, that point to the beginning of the list.

2. Increment p2 by n-1 positions, to make it point to the nth node
   from the beginning (to make the distance of n between p1 and p2).

3. Check for p2->next == null if yes return value of p1, otherwise
   increment p1 and p2.  If next of p2 is null it means p1 points to
   the nth node from the last as the distance between the two is n.

4. Repeat Step 3.
 */
List nthback(const List *lst, int n)
{
  if (*lst == NULL || n < 1) {
    return NULL;
  }
  
  struct elt *p1;
  struct elt *p2;

  p1 = p2 = *lst;

  for (int j = 0; j < n - 1; j++){
    if (p2 == NULL) {
      return NULL; // fewer than n elements
    }
    p2 = p2->next;
  }
  while (p2->next != NULL) {
    p1 = p1->next;
    p2 = p2->next;
  }
  return p1;
}


void deleteNode(List *lst)
{
  List tmp = (*lst)->next;
  (*lst)->value = (*lst)->next->value;
  (*lst)->next = (*lst)->next->next;
  free(tmp);
}

/* delete element with given value 

   Note: this creates an error if the value is in the last node of the list.
   How could you fix that?
*/


void delete(List *lst, int value)
{
  List e;

  List found = NULL;

  if (listEmpty(lst)) {
    fprintf(stderr, "Warning: empty list.\n");
  }

  for(e = *lst; e != 0; e = e->next) {

    if (e->value == value && e->next == NULL) {
      printf("Cannot delete last node of list.  Sorry.\n");
    } else if (e->value == value) {
      found = e->next;
      e->value = e->next->value;
      e->next = e->next->next;
      break;
    }
  }
  
  if (!found) {
    printf("No node with value: %d\n", value);
  } else {
    printf("Removed node with value: %d\n", value);
    free(found);
  }
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: lists integers+\n");
    exit(1);
  }

  List lst;

  lst = NULL;

  for (int i=1; i<argc; i++) {
    int val = atoi(argv[i]);
    printf("insert %d\n", val);
    insert(&lst, val);
    listPrint(&lst);
  }

  printf("Maximum value: %d\n", max(&lst));

  int len = length(&lst);
  printf("Length of list: %d\n", len);

  printf("Deleting node with value: %d\n", 1);
  delete(&lst,1);

  len = length(&lst);
  listPrint(&lst);
  printf("Element %d of list: %d\n", len-2, nth(&lst,len-2)->value);
  listPrint(&lst);

  printf("Deleting element %d of list.\n", len-2);
  List tmp = nth(&lst,len-2);
  deleteNode(&tmp);

  listPrint(&lst);
  printf("Element %d from end of list: %d\n", 2, nthback(&lst,2)->value);
  printf("Length of list: %d\n", length(&lst));

  printf("Mystery function:\n");
  mystery(&lst);
  printf("\n");

  while(!listEmpty(&lst)) {
    printf("pop gets %d\n", pop(&lst));
    listPrint(&lst);
  }

  printf("Length of list: %d\n", length(&lst));

  return 0;
}

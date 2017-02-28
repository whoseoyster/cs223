#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <ctype.h>
#include <stdbool.h>

int
strcmp(const char *s1, const char *s2)
{
    while(*s1 && *s2 && *s1 == *s2) {
        s1++;
        printf("s1: %c\n", *s1);
        s2++;
        printf("s2: %c\n", *s2);
    }

    return *s1 - *s2;
}

int main(){
  


  char foo[4] = "bruh";
  char *bar = "bruhv\0";


  if (strcmp(foo, bar) == 0) {
    printf("yessumb\n");
  } else {
    printf("noo\n");
  }

  qsort(w1, strlen(w1), sizeof(w1[0]), cmpfun)

  return 0;
}


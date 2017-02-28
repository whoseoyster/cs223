#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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

int main(int argc, char ** argv) {
  
  char word[4] = "rut";
  char word2[4] = "urr";
  
  qsort(word, strlen(word), sizeof(word[0]), cmpfunc);
  qsort(word2, strlen(word2), sizeof(word[0]), cmpfunc);

  if (strcmp(word, word2) == 0) {
    printf("yes\n");
  } else {
    printf("no\n");
  }

  return 0;
}

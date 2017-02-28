#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

bool str_rotation(char *s, char *s2)   {
    int i = 0;
    int j = 0;
    int len = 2*strlen(s);
    char new[len];
    memset(new, '\0', len);
     
    for(i = 0; s[i] != '\0'; i++)   {
        new[i] = s[i];
        new[i+(len/2)] = s[i];
    }
    if (strstr(new, s2))
        return true;
    return false;
}

int main(int argc, char ** argv) {
  
  char *word = "waterbottle";
  char *word2 = "erbottlewat";
  str_rotation(word, word2);
  return 0;
}

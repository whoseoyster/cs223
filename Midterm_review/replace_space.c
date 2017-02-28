#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void replace_space(char *s)   {
    int i = 0;
    int j = 0;
    int safe_len = 3*strlen(s);
    char new[safe_len];
    memset(new, '\0', safe_len);

    printf("%s\n", s);
     
    for(i = 0; s[i] != '\0'; i++)   {
        if (s[i]==' ') {
            new[j] = '%';
            j++;
            new[j] = '2';
            j++;
            new[j] = '0';
            j++;
        } else {
            new[j] = s[i];
            j++;
        }
    }
    printf("%s\n", new);
}

int main(int argc, char ** argv) {
  
  char *word = "pool is fun, right?";
  replace_space(word);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool removedupchar(char *s)   {
    int i = 0;
    int j = 0;
    int k = 0;
    int sameflag = 0;
    int len = strlen(s);
    char dup[len];
    memset(dup, '\0', len);

    printf("%s\n", s);
     
    for(i = 0; s[i] != '\0'; i++)   {
    	sameflag=0;
        for (j = 0; j < len; j++)    {
            if (dup[j]==s[i])
                sameflag = 1;
        }
        if(sameflag)
            continue;
        dup[k] = s[i];
        k++;
    }
    printf("%s\n", dup);
}

int main(int argc, char ** argv) {
  
  char *word = "pool";
  char *word2 = "loop";
  removedupchar(word);
  return 0;
}

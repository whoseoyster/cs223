#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char ** argv) {
  
  for(int i=0; i<23;i++) {
    printf("%i\n", i);
    if(i==5) {
        if(i+1 == 6) {
            break;
        }
    }
  }
  return 0;
}

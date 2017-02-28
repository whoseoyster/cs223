#include <stdio.h>
#include <stdlib.h>

void binPacking(int *a, int size, int n) {
    int binCount = 1, i;
    int s = size;
    for (i = 0; i < n; i++) {
        if (s - *(a + i) > 0) {
            s -= *(a + i);
            continue;
        } else {
            binCount++;
            s = size;
            i--;
        }
    }
 
    printf("Number of bins required: %d", binCount);
}
 
int main(int argc, char **argv) {
    printf("Enter the number of items in Set: ");
    printf("Enter %c items:", argv[1][0]);
    int n;    
    int a[n], i;
    int size;
    // n = atoi(argv[1]);
    scanf("%d", &n);
    printf("Enter %d items:", n);
 
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);
    printf("Enter the bin size: ");
    printf("%i \n", *(a+1));
    scanf("%d", &size);
    binPacking(a, size, n);
    return 0;
}

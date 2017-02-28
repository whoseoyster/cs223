/* 
 Pack.c

 CS 223b, hw2  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h2.v0.html

 Name: Rishab Ramanathan
 netid: rr566

 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

// sort the bin in decreasing order
int compare (const void* p1, const void* p2)
{ 
   int i1 = *(int*) p1;
   int i2 = *(int*) p2;
   if (i1 > i2) return -1;
   else if (i1 == i2) return 0;
   else return 1;
 }

//nextBin algorithm
int nextBinPacking(int *a, int size, int n, bool trace) {
    int binCount = 1, i;
    int s = size;

    //store in previous bin if it has adequate capacity, otherwise add to new empty bin
    for (i = 0; i < n; i++) {
        if (s - *(a + i) >= 0) {
            s -= *(a + i);
            if (trace) { 
              printf("arg: %i val: %i bin: %i total: %i\n", i,*(a+i),binCount-1, 100-s);
        }
            continue;
        } else {
            binCount++;
            s = size;
            i--;
        }
    }
    return binCount;
}

//firstBin algorithm
int firstBinPacking(int *a, int size, int n, bool trace) {
    int binCount = 1, i, j;
    int *bins;
    bins=malloc(n*sizeof(int));
    bins[0]=0;
    bool fit;

    //store in first bin that has adequate capacity
    for (i = 0; i < n; i++) {
        fit=false;
        for (j = 0; j < binCount; j++) {
          if (*(bins+j) + *(a + i) <= size) {
              *(bins+j)+= *(a + i);
              fit=true;
              break;
          }
        }
        if (!fit) {
            binCount++;
            bins[binCount-1] = *(a + i);
            j = binCount-1;
        }
        if (trace) { 
            printf("arg: %i val: %i bin: %i total: %i\n", i,*(a+i),j,bins[j]);
        }
    }
    free(bins);
    return binCount;
}

int bestBinPacking(int *a, int size, int n, bool trace) {
    int binCount = 1, i, j;
    int *bins;
    bins=malloc(n*sizeof(int));
    bins[0]=0;
    bool fit;
    int fit_bin;

    //store in bin that has least space and satisfies capacity constraint
    for (i = 0; i < n; i++) {
        fit=false;
        int fit_width = 0;
        for (j = 0; j < binCount; j++) {
          if ((bins[j] + *(a + i) <= size) && (bins[j] + *(a + i) > fit_width)) {
              fit_bin = j;
              fit_width = bins[j] + *(a + i);
              fit=true;
              continue;
          }
        }
        if (!fit) {
            binCount++;
            bins[binCount-1] = *(a + i);
            fit_bin = binCount-1;
        } else {
            bins[fit_bin]+= *(a + i);
        }
        if (trace) { 
            printf("arg: %i val: %i bin: %i total: %i\n", i,*(a+i),fit_bin,bins[fit_bin]);
        }
    }
    free(bins);
    return binCount;
}

//offline ffd algorithm that simply calls firstBin
int ffdBinPacking(int *a, int size, int n, bool trace) {
    return firstBinPacking(a, size, n, trace);
}

//offline bfd algorithm that simply calls bestBin
int bfdBinPacking(int *a, int size, int n, bool trace) {
    return bestBinPacking(a, size, n, trace);
}


//backtracking
int bp(int n, int total, int size, int binCount, int *lowestBinCount, int lastBin, int lowerBound, int *bins, int *a, bool trace) {
  int start_bin=0;
  int binCount_t = binCount;

  //return if all numbers placed in bins
  if(n==total) {
    *lowestBinCount = binCount_t;
    return binCount_t;
  }
  // order(bins, total); //meant to be capacity heuristic
  if (a[n-1] == a[n] && n != 0) {
    start_bin=lastBin;
  }

  //for all bins that can place the item
  for (int i=start_bin; i<total;i++) {
    if (i != 0) {
      // don't repeat previous bin of same size heuristic and don't try to add to already full bin
      if (bins[i] == bins[i-1] || bins[i] == 100) {
        continue;
      }
    }

    //add number to bin
    bins[i] += a[n];
    if (trace)
      printf("Added   arg: %i val: %i bin %i: total: %i\n", n, a[n], i, bins[i]);

    if (bins[i] > 100) {
      bins[i] -= a[n];
      if (trace)
        printf("Removed arg: %i val: %i bin %i: total: %i\n", n, a[n], i, bins[i]);
      continue;
    }

    //calculate bin count by checking all non-zero bins
    binCount_t=0;
    for (int j=0; j<total;j++) {
      if (bins[j] > 0 && bins[j] <= 100) {
        binCount_t++;
      }
    }

    //if partial solution is greater than lowest solution, stop
    if (binCount_t >= *lowestBinCount) {
      bins[i] -= a[n];
      if (trace)
        printf("Removed arg: %i val: %i bin %i: total: %i\n", n, a[n], i, bins[i]);
      break;
    }

    //if next item is same as current, don't place next item to the left of current bin heuristic
    lastBin = i;
    binCount_t = bp(n+1, total, size, binCount_t, lowestBinCount, lastBin, lowerBound, bins, a, trace);

    //if bin count is at lower bound, break out of for loop to return bin count
    if (binCount_t == lowerBound) {
      break;
    }
    
    //remove item from bin if partial solution does not work and try next bin
    bins[i] -= a[n];
    if (trace)
      printf("Removed arg: %i val: %i bin %i: total: %i\n", n, a[n], i, bins[i]);
  }
  if (binCount_t > *lowestBinCount) {
    return *lowestBinCount;
  }
  return binCount_t;
}

//called for optimal bin backing
int optmBinPacking(int *a, int size, int n, bool trace) {
    int bins[n];
    for (int j=0; j<n;j++) {
      bins[j] = 0;
    }
    int binCount = 0;
    int low=n;
    int *lowestBinCount;
    lowestBinCount = &low;
    float lower_bound = 0;
    for(int i=0;i<n;i++) {
      lower_bound += a[i];
    }
    lower_bound = lower_bound/(float)size;
    lower_bound = ceilf(lower_bound);
    int l_b = (int)lower_bound;

    //backtracking algorithm called
    binCount = bp(0, n, size, binCount, lowestBinCount, 0, l_b, bins, a, trace);
    // if (binCount != l_b) {
    //   binCount = l_b;
    // }

    return binCount;
}



int main(int argc, char **argv) {

    int n=0; //number of elements
    int *a, *temp, i;
    a=(int*)malloc(sizeof(int)); //allocate a memory on heap
    int size=100; //capacity of bin
    char first_dig; //for parsing arguments
    bool no_numbers = false, trace=false;
    int first=0, best=0, next=0, ffd=0, bfd=0, optm=0;
    char flags[500][7]; //to store flag names
    int flag_count = 0;
    bool ffd_pres=false,bfd_pres=false,optm_pres=false; //to check which offline flags are present
    bool next_pres=false,best_pres=false,first_pres=false;
    
    //check for arguments
    for (i = 1; i < argc; i++) {
      first_dig = argv[i][0];
      //if number, then add to array of numbers
      if (first_dig > '0' && first_dig <= '9') {
        for(int j=0; j<strlen(argv[i]); j++) {
          if (!(argv[i][j] >= '0' && argv[i][j] <= '9')) {
            fprintf(stderr, "Fatal error: Invalid size: %s\n", argv[i]);
            exit(0);
          }
        }
        int t_numb = atoi(argv[i]);
        if (!no_numbers) {
          n++;
          if (0 <= t_numb && t_numb <=100) {
            a[i-1] = t_numb;
            //allocate more space on heap for number array
            temp=(int*)realloc(a,(n+1)*sizeof(int));
            if ( temp != NULL ) {
              a=temp;
            } else {
              free(a);
              printf("Error allocating memory!\n");
              return 1;
            }
          } else {
            fprintf(stderr, "Fatal error: Invalid size: %i\n", t_numb);
            return 1;
          }
        } else {
          fprintf(stderr, "Fatal Error: Size option out of order: %i\n", t_numb);
          return 1;
        }
      } else {
        //no more numbers should appear after this. Only flags and trace. Run online algorithms here.
        no_numbers = true;
        if (strcmp(argv[i], "-next") == 0) {
            strcpy(flags[flag_count], "-next");
            next_pres=true;
        } else if (strcmp(argv[i], "-first") == 0) {
            strcpy(flags[flag_count], "-first");
            first_pres=true;
        } else if (strcmp(argv[i], "-best") == 0) {
            strcpy(flags[flag_count], "-best");
            best_pres=true;
        //offline algorithms meant to be run later on. For now, simply stored in order
        } else if (strcmp(argv[i], "-ffd") == 0) {
            strcpy(flags[flag_count], "-ffd");
            ffd_pres=true;
        } else if (strcmp(argv[i], "-bfd") == 0) {
            strcpy(flags[flag_count], "-bfd");
            bfd_pres=true;
        } else if (strcmp(argv[i], "-optm") == 0) {
            strcpy(flags[flag_count], "-optm");
            optm_pres=true;
        } else if (strcmp(argv[i], "-trace") == 0) {
            trace=true;
            flag_count--;
        } else {
            char sec_dig = argv[i][1];
            //for negative numbers
            if (sec_dig > '0' && sec_dig <= '9') {
              fprintf(stderr, "Fatal error: Invalid size: %s\n", argv[i]);
              return 1;
            } else {
              fprintf(stderr, "Fatal error: bad argument: %s\n", argv[i]);
              return 1;
            }
        }
        //total no. of flags given
        flag_count++;
      }
    }
    
    if (flag_count == 0) {
      fprintf(stderr, "Fatal error: no algorithm specified.\n");
      return 1;
    }

    if (next_pres) {
        if (trace)
          printf("Trace -next\n");
        next = nextBinPacking(a, size, n, trace);
    }

    if (first_pres) {
        if (trace)
          printf("Trace -first\n");
        first = firstBinPacking(a, size, n, trace);
    }

    if (best_pres) {
        if (trace)
          printf("Trace -best\n");
        best = bestBinPacking(a, size, n, trace);
    }

    //ready for offline processing
    qsort( a, n, sizeof(int), compare );

    //run the offline algorithms
    if (ffd_pres) {
        if (trace)
          printf("Trace -ffd\n");
        ffd = ffdBinPacking(a, size, n, trace);
    }

    if (bfd_pres) {
        if (trace)
          printf("Trace -bfd\n");
        bfd = bfdBinPacking(a, size, n, trace);
    }

    if (optm_pres) {
        if (trace)
          printf("Trace -optm\n");
        optm = optmBinPacking(a, size, n, trace);
    }

    //print out results in order of flags in arguments
      for (i=0;i<flag_count;i++) {
        if (strcmp(flags[i], "-next") == 0) {
            printf("-next: %i\n", next);
        } else if (strcmp(flags[i], "-first") == 0) {
            printf("-first: %i\n", first);
        } else if (strcmp(flags[i], "-best") == 0) {
            printf("-best: %i\n", best);
        } else if (strcmp(flags[i], "-ffd") == 0) {
            printf("-ffd: %i\n", ffd);
        } else if (strcmp(flags[i], "-bfd") == 0) {
            printf("-bfd: %i\n", bfd);
        } else if (strcmp(flags[i], "-optm") == 0) {
            printf("-optm: %i\n", optm);
        } else {
            break;        
        }
      }

    //free memory
    free(a);
    return 0;
}

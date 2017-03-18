/* 
Calc.c

 CS 223b, hw4  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h4.v0.html

 Name: Rishab Ramanathan
 netid: rr566

 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define ungetchar(c)  ungetc(c, stdin)    // unread char read from stdin

// // here is where you can gobble up line splices
// int mygetchar() {
//   int ch = getchar();
//   while (ch == '\\') {
//     ch = getchar();
//     //disregard anything immediately after the \ character
//     ch = getchar();
//   }
//   return ch;
// }
// //converts ASCII string to long
// long converter(char s[], int num_type, char ch)
// {
//   int i; 
//   long n;
//   char l;
//   n = 0;
//   ungetchar(ch); //put back the last read character
  
//   //set the character upper limit based on the base type
//   switch (num_type) {
//     case 2:
//       l = '1';
//       break;
//     case 10:
//       l = '9';
//       break;
//     case 8:
//       l = '7';
//       break;
//   }

//   if (num_type != 16) {
//     for (i=0; s[i] >= '0' && s[i] <= l; ++i) {
//       n = num_type * n + (s[i] - '0');
//     }
//   } else {
//     for (i=0; isxdigit(s[i]) ; ++i)
//       if (isdigit(s[i])) {
//         n = num_type * n + (s[i] - '0');
//       } else {
//         n = num_type * n + (tolower(s[i]) - 'a' + 10);
//       }    
//   }
//   return n;
// }

int main(int argc, char **argv) {
  //debug statement
  bool debug = false;
  if (argc == 2 && strcmp(argv[1], "-debug") == 0) {
    debug = true;
  }
  //declare base type variables
  char in[1024];
  // in = (char *)malloc(100*sizeof(char));
  // char *initial = in;
  char c;
  bool d = false;
  int i=0;

  while (true) {
    while((c = getchar()) != '\n' && c != EOF) {
      // printf("%c\n", c);
      in[i] = c;
      i++;
      d = true;
    }
    // in = initial;
    if (d) {
      printf("Input: %s\n", in);
      d = false;
      i=0;
      for (int j=0;j<1024;j++) {
        if (in[j] == '\0') {
          break;
        }
        in[j] = '\0';
      }
    }
    // c = '0';
  }

  // while ((ch = mygetchar()) != EOF){
  //   neg = false;
  //   add = 0;
  //   //for negative values
  //   if (ch == '-') {
  //     neg = true;
  //     while ((ch = mygetchar()) == '-') {
  //       //do nothing
  //     }
  //   } 
  //   //gobble up quotation marks and anything within them
  //   if (ch == '\"') {
  //     while ((ch = mygetchar()) != EOF){
  //       if (ch == '\"') {
  //         break;
  //       }
  //     }
  //   }
  //   //handle all digits here
  //   if (isdigit(ch)) {
  //     //30 characters is large enough to contain the longest string
  //     char number[70] = " "; 
  //     if (ch == '0') {
  //       ch = mygetchar();
  //       ch = tolower(ch);
  //       //handle binary
  //       if (ch == 'b') {
  //         size_t len = 0;  
  //         while ((ch = mygetchar()) >= '0' && ch <= '1') {
  //           number[len] = ch;
  //           len++;
  //         }
  //         add = converter(number, bin, ch);
  //       }
  //       //handle hexadecimal
  //       else if (ch == 'x') {
  //         size_t len = 0;  
  //         while (isxdigit((ch = mygetchar()))) {
  //           number[len] = ch;
  //           len++;
  //         }
  //         add = converter(number, hex, ch);
  //       }
  //       //handle octal
  //       else if (ch >= '0' && ch <= '7') {
  //         ungetchar(ch);
  //         size_t len = 0;
  //         while ((ch = mygetchar()) >= '0' && ch <= '7') {
  //           number[len] = ch;
  //           len++;
  //         }
  //         add = converter(number, oct, ch);
  //       }
  //     }
  //     //handle decimal
  //     else {        
  //       ungetchar(ch);
  //       size_t len = 0;  
  //       while ((ch = mygetchar()) >= '0' && ch <= '9') {
  //         number[len] = ch;
  //         len++;
  //       }
  //       add = converter(number, dec, ch);
  //     }
  //   }
  //   if (neg) {
  //     add = -add; //switch sign of added number
  //   }      
  //   total = total + add;
  //   if (debug && (add != 0)) {
  //     printf("n: %ld tot: %ld\n", add, total);
  //   }
  // }  
  // printf("Total: %ld\n", total);
  return 0;

}

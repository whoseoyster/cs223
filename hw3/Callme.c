/* 
 Callme.c

 CS 223b, hw23 
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h3.v0.html

 Name: Rishab Ramanathan
 netid: rr566

 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

#include "/c/cs223/hw3/hash.h"

struct elt {
  struct elt *next;
  char *key;
};

struct hash {
  int size;           // size of the pointer table 
  int n;              // number of elements stored 
  int buckets;
  struct elt **table;
};


int HashGetNumber(Hash d)
{
    return d->n;
}

void iterate(char *str, int idx, int len, char *orig, Hash h, int *found) {
    char c;

    char start;
    char end;

    if (orig[idx] == '0' || orig[idx] == '1') {
        printf(" ** no matches **\n");
        exit(0);
    }
    if (orig[idx] == '2') {
        start = 'a';
        end = 'c';
    } else if (orig[idx] == '3') {
        start = 'd';
        end = 'f';
    } else if (orig[idx] == '4') {
        start = 'g';
        end = 'i';
    } else if (orig[idx] == '5') {
        start = 'j';
        end = 'l';
    } else if (orig[idx] == '6') {
        start = 'm';
        end = 'o';
    } else if (orig[idx] == '7') {
        start = 'p';
        end = 's';
    } else if (orig[idx] == '8') {
        start = 't';
        end = 'v';
    } else if (orig[idx] == '9') {
        start = 'w';
        end = 'z';
    }
    if (idx < (len - 1)) {
        for (c = start; c <= end; ++c) {
            str[idx] = c;

            iterate(str, idx + 1, len, orig, h, found);
        }
    } else {

        for (c = start; c <= end; ++c) {
            str[idx] = c;

            bool item;
            item = HashSearch(h, str);
            
            if(item) {
                printf(" %s", str);

                if(*found ==0) {
                    *found = 1;
                }
            }
        }
    }
}

void convertAlpha(char *seq, int length, char *numbers) {
    char l;
    for (int i=0; i<strlen(seq); i++) {
        l = seq[i];
        if (l == 'a' || l == 'b' || l == 'c') {
            numbers[i] = '2';
        } else if (l == 'd' || l == 'e' || l == 'f') {
            numbers[i] = '3';
        } else if (l == 'g' || l == 'h' || l == 'i') {
            numbers[i] = '4';
        } else if (l == 'j' || l == 'k' || l == 'l') {
            numbers[i] = '5';
        } else if (l == 'm' || l == 'n' || l == 'o') {
            numbers[i] = '6';
        } else if (l == 'p' || l == 'q' || l == 'r' || l == 's') {
            numbers[i] = '7';
        } else if (l == 't' || l == 'u' || l == 'v') {
            numbers[i] = '8';
        } else if (l == 'w' || l == 'x' || l == 'y' || l == 'z') {
            numbers[i] = '9';
        }
    }
}

int main(int argc, char **argv) {

    char *dictionary = "/usr/share/dict/words";
    bool alpha=false;

    debugflag = false;
    if (argc<2 || argc>5) {
      fprintf(stderr, "usage: Callme (digits | letters) [-debug?] [-file filename]\n" );
      exit(0);
    }

    //turn on debug
    if (argc == 3) {
        if(strcmp(argv[argc-1], "-debug") == 0) {
            debugflag=true;
        } else {
            fprintf(stderr, "Fatal error: invalid argument %s\n", argv[argc-1]);
            exit(0);
        }
    } else if (argc == 5) {
        if (strcmp(argv[argc-3], "-debug") == 0) {
            debugflag = true;
            if (strcmp(argv[argc-2], "-file") == 0) {
                dictionary = argv[argc-1];
            } else {
                fprintf(stderr, "Fatal error: invalid argument %s\n", argv[argc-2]);
                exit(0);
            }
        } else {
            fprintf(stderr, "Fatal error: invalid argument %s\n", argv[argc-3]);
            exit(0);
        }
    } else if (argc == 4) {
        if (strcmp(argv[argc-2], "-file") == 0) {
            dictionary = argv[argc-1];
        } else {
            fprintf(stderr, "Fatal error: invalid argument %s\n", argv[argc-2]);
            exit(0);
        }
    }

    char seq[strlen(argv[1]+1)];
    strcpy(seq, argv[1]);

    int size = 0;
    size = strlen(argv[1]);

    if (isalpha(seq[0])) {
        for (int i=0; i<size;i++) {
            if(!isalpha(seq[i])) {
                fprintf(stderr, "Fatal error: invalid argument %s\n", seq);
                exit(0);
            }
        }
        alpha=true;
    } else {
        for (int i=0; i<size;i++) {
            if(!atoi(&seq[i]) || atoi(&seq[i]) == 0 || (seq[i] == '-')) {
                fprintf(stderr, "Fatal error: invalid argument %s\n", seq);
                exit(0);
            }
        }
    }

    Hash h = HashCreate();

    if(alpha) {
        for(int i = 0; seq[i]; i++){
            seq[i] = tolower(seq[i]);
        }
        char numbers[256] = "s";
        convertAlpha(seq, size, numbers);
        if (numbers[0] != 'a') {
            printf("alphabetic: %s => %s\n", argv[1], numbers);
        }
    } else {

        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        unsigned int read;

        fp = fopen(dictionary, "r");
        if (fp == NULL) {
            fprintf(stderr, "Fatal error: dictionary not found.\n");
            HashDestroy(h);
            exit(0);
        }
        if(debugflag) {
            printf("Loading dictionary: %s\n", dictionary);
        }

        while ((read = getline(&line, &len, fp)) != -1) {
            line[strlen(line)-1] = 0;
            bool alpha=true;
            for(int i = 0; line[i]; i++){
                if (!isalpha(line[i])) {
                    alpha=false;
                    break;
                }
                line[i] = tolower(line[i]);
            }
            if (read == size+1 && alpha) {
              HashInsert(h, line);
            }
        }

        if(debugflag)
            printf("Word Count: %i\n", HashGetNumber(h));

        fclose(fp);
        if (line)
          free(line);
        
        printf("numeric: %s =>", seq);

        char str[size + 1];
        int j = 0;
        int *found = &j;

        memset(str, 0, size + 1);

        iterate(str, 0, size, seq, h, found);
        if (*found == 0) {
            printf(" ** no matches **");
        }
        putchar('\n');
    }
    HashDestroy(h);
    return 0;
}

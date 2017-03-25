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
#include <assert.h>
#include "stack.h"

#define stacksize (1024)

void * mymalloc(size_t s){
  void * ptr = malloc(s);
  // printf("Mymalloc: %p\n", ptr);
  return ptr;
}

void calculate(stackT *stackP) {

  int s = stackP->top + 1;

  stackT stO;
  StackInit(&stO, stacksize);

  if (debugflag)
    printf("OUTPUT:\n");

  Token t1;
  Token oper1;
  Token oper2;
  Token result;

  for(int i=0;i<s;i++) {
  
    // t1 = mymalloc(sizeof(struct token));
    // assert(t1);

    t1 = stackP->contents[i];

    if (debugflag) {
      StackPrint(&stO);
      printf("Token:%s: type: %i value: %.2f\n", t1->str, t1->type, t1->value);
    }

    if (t1->type == 0) {
      StackPush(&stO, t1);
      continue;
    } 
    
    double result = 0;

    oper1 = StackPop(&stO);
    oper2 = StackPop(&stO);

    if (strcmp(t1->str, "+") == 0) {
      result = oper1->value + oper2->value;
    } else if (strcmp(t1->str, "-") == 0) {
      result = oper2->value - oper1->value;
    } else if (strcmp(t1->str, "*") == 0) {
      result = oper1->value * oper2->value;
    } else if (strcmp(t1->str, "/") == 0) {
      result = oper2->value / oper1->value;
    }
    free(oper2->str);
    free(oper2);
    free(oper1->str);
    free(oper1);

    result = mymalloc(sizeof(struct token));

    result->value = result;
    char str[1024];
    sprintf(str, "%d", (int)result );

    result->str = strdup(str);
    StackPush(&stO, result);
    
  }

  stackP->top = -1;

  printf("Result: %.2f\n", StackTop(&stO)->value);
  StackDestroy(&stO);
  StackDestroy(stackP);

}

int main(int argc, char **argv) {
  //debug statement
  debugflag = false;
  if (argc == 2 && strcmp(argv[1], "-debug") == 0) {
    debugflag = true;
  }
  //declare input processing variables
  char in[1024];
  char c;
  bool d = false;
  int i=0;

  char *tok;
  double val = 0;

  memset(in, '\0', 1024);

  Token tk;
  Token t;
  Token top;

  while (true) {
    while((c = getchar()) != '\n' && c != EOF) {
      in[i] = c;
      i++;
      d = true;
    }

    if (d) {
      printf("Input: %s\n", in);
      d = false;
      i=0;

      tok = strtok(in, " ");

      stackT st1;
      stackT st2;
      StackInit(&st1, stacksize);
      StackInit(&st2, stacksize);

      if (debugflag) {
        StackPrint(&st1);
      }

      while( tok != NULL ) 
      {
        // printf(" %s\n", tok );

        t = mymalloc(sizeof(struct token));
        assert(t);

        val = 0;
        t->str = strdup(tok);
        t->value = val;

        if (strcmp(tok, "(") == 0) {
          t->type = LPAR;
          StackPush(&st1, t);
        } else if (strcmp(tok, ")") == 0) {
          t->type = RPAR;
          top = StackTop(&st1);
          while (top->type != LPAR) {
            StackPop(&st1);
            StackPush(&st2, top);
            if(StackIsEmpty(&st1)) {
              fprintf(stderr, "Error: Mismatched parentheses\n");
              exit(0);
            }
            top = StackTop(&st1);
          }
          StackPop(&st1);
          free(top->str);
          free(top);
        } else if (strcmp(tok, "+") == 0) {
          t->type = OP1;
        } else if (strcmp(tok, "-") == 0) {
          t->type = OP1;
        } else if (strcmp(tok, "/") == 0) {
          t->type = OP2;
        } else if (strcmp(tok, "*") == 0) {
          t->type = OP2;
        } else {
          char *extra;
          double num = strtod(tok, &extra);
          if (strlen(extra) != 0) {
            fprintf(stderr, "Fatal Error. Bad token: %s\n", tok);
            exit(0);
          } else {
            t->type = NUM;
            val = num;
            t->value = val;
            StackPush(&st2, t);
          }
        } 
        if (t->type > 0 && t->type < 3) {
          if (StackCount(&st1) > 0) {
            top = StackTop(&st1);
            while (top->type >= t->type && top->type < 3) {
              StackPop(&st1);
              StackPush(&st2, top);
              if(StackIsEmpty(&st1)) break;
              top = StackTop(&st1);
            }
          }
          StackPush(&st1, t);  
        } 

        if (debugflag) {
          printf("Token:%s: type: %i value: %.2f\n", t->str, t->type, t->value);
          StackPrint(&st1);
        }

        if (t->type == RPAR) {
          free(t->str);
          free(t);
        }

        tok = strtok(NULL, " ");
      }
      
      while (!StackIsEmpty(&st1)) {
        tk = StackPop(&st1);
        if (tk->type == 3) {
          fprintf(stderr, "Error: Mismatched parentheses\n");
          exit(0);
        }
        StackPush(&st2, tk);
      }
      
      StackDestroy(&st1);

      calculate(&st2);

      memset(in, '\0', 1024);
    }
  }

  return 0;

}

/* 
stack.c

 CS 223b, hw4  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h4.v0.html

 Name: Rishab Ramanathan
 netid: rr566

 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "stack.h"

/* print contents of stack on a single line */
void
StackPrint(stackT *stackP)
{
	int s = stackP->top + 1;
	Token t;

    printf("Stack: size: %i :", s);

    for(int i=s-1;i>-1;i--) {
    	t = stackP->contents[i];
    	printf("[%i %s %.1f] ", t->type, t->str, t->value);
    }

    putchar('\n');
}

void 
StackInit(stackT *stackP, int maxSize) 
{
	Token *newContents;

	/* Allocate a new array to hold the contents. */

	newContents = (Token *)malloc(sizeof(Token)
	                                    * maxSize);
	assert(newContents);

	stackP->contents = newContents;
	stackP->maxSize = maxSize;
	stackP->top = -1;  /* I.e., empty */
}

void
StackDestroy(stackT *stackP)
{
	/* Get rid of array. */
	for (int i=0; i<stackP->top+1; i++) {
		if (stackP->contents[i]->str != NULL) {
			free(stackP->contents[i]->str);
		}
		if (stackP->contents[i] != NULL) {
			free(stackP->contents[i]);
		}
	}

	free(stackP->contents);
	
	stackP->contents = NULL;
	stackP->maxSize = 0;
	stackP->top = -1;  /* I.e., empty */
}

bool
StackIsEmpty(stackT *stackP)
{
    return stackP->top < 0;
}

bool 
StackIsFull(stackT *stackP)
{
	return stackP->top >= stackP->maxSize - 1;
}

/* push a new value onto top of stack */
void
StackPush(stackT *stackP, Token element)
{
    if(StackIsFull(stackP)) {
    	fprintf(stderr, "Can't push element to stack. Stack is full.\n");
    	exit(1);
    }

    stackP->contents[++stackP->top] = element;
}

Token
StackPop(stackT *stackP)
{
	if (StackIsEmpty(stackP)) {
		fprintf(stderr, "Fatal error: fewer than 2 operands available.\n");
		exit(1);  /* Exit, returning error code. */
	}
	return stackP->contents[stackP->top--];
}

// number of elements on the stack
int 
StackCount(stackT *stackP) 
{
	return stackP->top + 1;
}

// Token at the top of stack
Token 
StackTop(stackT *stackP) {
	return stackP->contents[stackP->top];
}

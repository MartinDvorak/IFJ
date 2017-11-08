#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

#ifndef SSTACK
typedef struct stack{
	char* bottom;
	int top;
	int max;
}Tstack;
#endif
#define SSTACK

void stack_realloc(Tstack* s);

Tstack *stack_init();

char pop(Tstack* s);

char top_stack_terminal(Tstack* s);

void stack_push_lt(Tstack* s);

void printstack(Tstack* s);

void push(Tstack*s, char insert);

void free_stack(Tstack*s);

char top_stack(Tstack* s);

char read_top_pop(Tstack* s);

int stack_empty(Tstack* s);
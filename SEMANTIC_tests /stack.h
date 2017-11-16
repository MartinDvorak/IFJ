#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

#ifndef SSTACK
typedef struct stack{
	int* bottom;
	int top;
	int max;
}Tstack;
#endif
#define SSTACK

void stack_realloc(Tstack* s);

Tstack *stack_init();

int pop(Tstack* s);

int top_stack_terminal(Tstack* s);

void stack_push_lt(Tstack* s);

void printstack(Tstack* s);

void push(Tstack*s, int insert);

void free_stack(Tstack*s);

int top_stack(Tstack* s);

int read_top_pop(Tstack* s);

int stack_empty(Tstack* s);

int top2_stack(Tstack* s);

void pop2(Tstack*s);

void flush_stack(Tstack* s);

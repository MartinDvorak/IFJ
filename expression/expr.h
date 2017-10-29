#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

typedef struct stack{
	char* bottom;
	int top;
	int max;
}Tstack;


void stack_realloc(Tstack* s);

Tstack* stack_init();

char pop(Tstack* s);

char top_stack_terminal(Tstack* s);

void stack_push_lt(Tstack* s);

void printstack(Tstack* s);

void push(Tstack*s, char insert);

void free_stack(Tstack*s);

int reduce(Tstack*s);

char table_precedence(char** table,char token_symbol, char stack_symbol);

char **init_table_precedence();

void free_table(char **table);

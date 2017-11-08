#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define TRUE 1
#define FALSE 0


int reduce(Tstack*s);

char table_precedence(char** table,char token_symbol, char stack_symbol);

char **init_table_precedence();

void free_table(char **table);

int expr(char* string);

char next(char* string, int *position);
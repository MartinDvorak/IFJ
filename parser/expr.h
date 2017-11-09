#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define TRUE 1
#define FALSE 0


int reduce(Tstack*s);

char table_precedence(char** table,char token_symbol, char stack_symbol, int conditin);

char **init_table_precedence(int conditin);

void free_table(char **table, int conditin);

int expr(char* string, int conditin);

char next(char* string, int *position);
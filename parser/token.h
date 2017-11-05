#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#ifndef TTOKEN
typedef struct token{
	int type;
	
	int int_v;
	float float_v;
    char *string;

}TToken;
#endif
#define TTOKEN

TToken* token_init();

void token_free(TToken* t);

TToken* give_me(TToken* t);
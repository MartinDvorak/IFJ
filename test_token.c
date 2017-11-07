#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "stack.h"

int main(int argc, char **argv)
{
    TToken* token = NULL;
    Tstack* s = NULL;


    token = get_next(token, s);
    printf("####TOKEN 1####\n");
    printf("%d\n", token->type);
    printf("%s\n", token->string);
    printf("%ld\n", token->int_v);
    printf("%f\n\n", token->float_v);
    printf("###############\n");
    token = get_next(token, s);
    printf("####TOKEN 2####\n");
    printf("%d\n", token->type);
    printf("%s\n", token->string);
    printf("%ld\n", token->int_v);
    printf("%f\n", token->float_v);
    printf("###############\n");
    token_free(token);
    

    return 0;
}
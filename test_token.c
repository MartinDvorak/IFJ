#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "stack.h"

int main(int argc, char **argv)
{
    TToken* token = NULL;
    Tstack* s = NULL;
    int *storage = NULL;
    int i = 1; 

    storage = storage_init();

    while(1) {
        token = get_next(token, s, storage);
        printf("####TOKEN %d####\n", i);
        printf("%d\n", token->type);
        printf("%s\n", token->string);
        printf("%ld\n", token->int_v);
        printf("%f\n", token->float_v);
        printf("###############\n\n");
        i++;
        if (token->type == EOF) {
            break;
        }
    }

    token_free(token);

    storage_free(storage);

    return 0;
}
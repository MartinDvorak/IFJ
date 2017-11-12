#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "stack.h"




int main(int argc, char **argv)
{
    TToken* token = NULL;
    Tstack* s = NULL;
    int storage = -2;
    int i = 1; 

    while(1) {
        token = get_next(token, s, &storage);
        if (token->type==EOL)
          
    //    printf("%s\n", token->string);
    //    printf("%ld\n", token->int_v);
    //   printf("%f\n", token->float_v);

        i++;
        if (token->type == EOF) {
            break;
        }
    }


    printf ("\n");
    return 0;
}
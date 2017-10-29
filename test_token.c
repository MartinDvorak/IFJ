#include "token.h"

#define SIZEUNEED 10

int main(int argc, char **argv)
{
    TToken* token = NULL;
    FILE *fd;

    fd = fopen("text.txt", "r");

    token = get_next(token, fd);
    printf("%s\n", token->string);
    printf("%d\n", token->int_v);
    printf("%f\n", token->float_v);
    token = get_next(token, fd);
    printf("%s\n", token->string);
    printf("%d\n", token->int_v);
    printf("%f\n", token->float_v);
    token_free(token);
    
    fclose(fd);

    return 0;
}
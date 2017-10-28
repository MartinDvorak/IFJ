#include "token.h"

#define SIZEUNEED 10

int main(int argc, char **argv)
{
    TToken* token = NULL;
    FILE *fd;

    fd = fopen("text.txt", "r");

    token = get_next(token, fd);
    printf("%s\n", token->string);
    token = get_next(token, fd);
    printf("%s\n", token->string);
    token_free(token);
    
    fclose(fd);

    return 0;
}
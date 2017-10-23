#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

/*inicializuje string pro hodnotu tokenu, zakladni delka je 20 znaku
*/
void token_init(t_token *t) {
    
    t->length = 20;

    if ((t->value = (char *) malloc(t->length * sizeof(char))) == NULL)
        exit(-1);
}

/*uvolnuje alokovany token
*/
void token_destr(t_token *t) {
    
    free(t->value);
    free(t);
}

void value_realloc(t_token *t) {
    
        t->length += 20;
        if ((t->value = realloc(t->value, t->length)) == NULL)
            exit(-1);
    }

/*prida nacteny znak ze vstupu na konec retezce 
**pokud delka presahne hodnotu 20 zvetsi se     
**TODO zaroven s pomocnym retezcem zvetsit i retezec v tokenu
*/
void char_append(char c, t_token *t) {

    int len = strlen(t->value) + 1;
    
    if (len >= t->length) {
        value_realloc(t);
    }
    t->value[len - 1] = c;               
    t->value[len] = '\0';
}

/*v promenne t vraci dalsi token
**TODO navratova hodnota v pripade erroru
*/
int get_next_token(FILE *fd, t_token *t) {

    /*const char *keywords[] = {"As", "Asc", "Declare", "Dim", "Do", "Double", "Else",
        "End", "Chr", "Function", "If", "Input", "Integer", "Length", "Loop", "Print",
        "Return", "Scope", "String", "SubStr", "Then", "While", "And", "Boolea",
        "Continue", "Elseif", "Exit", "False", "For", "Next", "Not", "Or", "Shared"
        "Static", "True"};
    */
    int c;
    int state;

    //prochazi zdrojovym souborem dokud nenarazi na konec souboru
    //TODO zbytek automatu
    while ((c = getc(fd)) != EOF) {
        if (c >= '0' && c <= '9') {
            state = INTEGER;
            char_append(c, t);
        }       
    }
    
    t->type = state; //predava finalni stav a hodnotu do tokenu

    return 0;
}
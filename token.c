#include <ctype.h>
#include "token.h"


TToken* token_init()
{
	TToken* t;

	if ((t = malloc(sizeof(TToken))) == NULL)
		exit(-1);
	t->int_v = 0;
	t->float_v = 0.0;
	t->string = NULL;
	return t;
}

void token_free(TToken* t)
{
	if (t->string != NULL)
		free(t->string);
	free(t);
}

void char_append(char *str, char c) {

    int len = strlen(str);

    if (len + 1 >= ((len / 10 + 1) * 10)) {  //TODO comment
        str = realloc(str, (len + 1 + SIZEUNEED) * sizeof(char));
    }

    str[len] = c;
    str[len + 1] = '\0';
}

TToken* get_next (TToken* t, FILE *fd) {       // simuluje cinost lex.analyzatoru

    /*const char *keywords[] = {"As", "Asc", "Declare", "Dim", "Do", "Double", "Else",
    "End", "Chr", "Function", "If", "Input", "Integer", "Length", "Loop", "Print",
    "Return", "Scope", "String", "SubStr", "Then", "While", "And", "Boolean",
    "Continue", "Elseif", "Exit", "False", "For", "Next", "Not", "Or", "Shared"
    "Static", "True"};
    */
    char c;
    int state = 0;
    
    if (t == NULL) {                //FIRST USE
		t = token_init();	
	}
	
	free(t->string);
	if ((t->string = malloc(sizeof(char)*SIZEUNEED)) == NULL) {
        exit(-1);
    }

    while((c = getc(fd)) != EOF) {
        if (c >= 48 && c <= 57) {
            state = INT_V;
            char_append(t->string, c);
        }
        if (isblank(c))
            break;
    }

	return t;
}

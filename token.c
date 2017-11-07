#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include "stack.h"


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


void just_pop(Tstack* s) {

    s->top--;
}

TToken* get_next (TToken* t, Tstack* s) {       // simuluje cinost lex.analyzatoru

    const char *keywords[] = {"as", "asc", "declare", "dim", "do", "double", "else",
    "end", "chr", "function", "if", "input", "integer", "length", "loop", "print",
    "return", "scope", "string", "substr", "then", "while", "and", "boolean",
    "continue", "elseif", "exit", "false", "for", "next", "not", "or", "shared",
    "static", "true"};
    
    char c;
    int state = 0;
    char *tmp_s = NULL;
    
    if (t == NULL) {                //FIRST USE
        t = token_init();
    }
    
    free(t->string);
    
    if (s == NULL) {
        s = stack_init();
    }
    else {
        while (!stack_empty(s)) {
            just_pop(s);
        }
    }

    c = getchar();
    c = tolower(c);


    if (c == '_') {
        state = ID;
        push(s,c);
    }

    else if (c >= 'a' && c <= 'z') {
        state = ID;
        push(s,c);
    }
        
    else if (c >= '0' && c <= '9') {
        state = INT_V;
        push(s, c);
    }

    else if (c == '!') {
        state = EXCL_M;
    }

    else if (c == ',') {
        state = COLON;
    }

    else if (c == ';') {
        state = SEMICOLON;
    }

    else if (c == '=') {
        state = ASSIGN;
    }

    else if (c == '<') {
        state = LESS;
    }

    else if (c == '>') {
        state = GREAT;
    }

    else if (c == '+') {
        state = ADD;
    }
    
    else if (c == '-') {
        state = SUB;
    }

    else if (c == '*') {
        state = MUL;
    }

    else if (c == '/') {
        state = DIV;
    }

    else if (c == '\\') {
        state = INTDIV;
    }
    //#TODO if (EOL)

    else if (c == '(') {
        state = BRACKET_L;
    }

    else if (c == ')') {
        state = BRACKET_R;
    }

    else if (c == '\'') {
        state = L_COMMENT;
    }

    else {
        //#TODO
    }

    printf("state(IN) = %d\n", state);

    while((c = getchar()) != EOF) {

        if (isblank(c)) {
            if (state == EXCL_M) {
                state = SCAN_ERR;
                break;
            }
            if (state != STRING_V) {
                break;
            }
        }
        printf("%d\n", c);

        if (c == 10) {      //skips line feed character
            continue;
        }

        switch (state) {

            case ID:
                if (c == '_') {
                    push(s, c);
                }
                else if (c >= 'a' && c <= 'z') {
                    push(s, c);
                }
                else if (c >= '0' && c <= '9') {
                    push(s,c);
                }
                else {
                    state = SCAN_ERR;
                }
                break;
            
            case INT_V:
                if (c >= '0' && c <= '9') {
                    push(s,c);
                }
                else {
                    state = SCAN_ERR;
                }
                break;
            
            case FLOAT_V:
                //#TODO
                break;

            case EXCL_M:
                if (c == '\"') {
                    state = STRING_V;
                    while ((c = getchar()) != '\"') {
                        push(s, c);
                    }
                    c = getchar();
                    break;
                }
                else {
                    state = SCAN_ERR;
                }
                break;

            case ASSIGN:
                if (c == '=') {
                    state = EQ;
                }
                break;

            case LESS:
                if (c == '=') {
                    state = LESSEQ;
                }
                else if (c == '>') {
                    state = NEQ;
                }
                break;
            
            case DIV:
                if (c == '/') {
                    state = B_COMMENT;
                }
                break;

            case L_COMMENT:
                while (getchar() != 10) {   //10 == linefeed
                    continue;
                }
                getchar();                  //to get line feed
                break;
            
            case B_COMMENT:
                //TODO
               //while (getchar() != )
               break;
        }
    }

    if ((tmp_s = malloc(sizeof(char)*(s->top + 1) + 1)) == NULL) {
        exit(-1);
    }

    strcpy(tmp_s, s->bottom);
    strcat(tmp_s, "\0");

    if (state == ID) {
        for (int i = 0; i < 35; i++) {
            if ((strcmp(tmp_s, keywords[i])) == 1) {
                state = i + AS;
            }
        }
    }

    printf("state(OUT) = %d\n", state);

    t->type = state;

    if (state == INT_V) {
        t->int_v = strtol(tmp_s, NULL, 10);
    }

    if ((t->string = malloc(sizeof(char)*(s->top + 1) + 1)) == NULL) {
        exit(-1);
    }

    if (state == ID || state == STRING_V) {
        strcpy(t->string, tmp_s);
    }

    free(tmp_s);
    free_stack(s);    

	return t;
}

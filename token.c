#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include "stack.h"


TToken* token_init () {
	TToken* t;

	if ((t = malloc(sizeof(TToken))) == NULL)
		exit(-1);
	t->int_v = 0;
	t->float_v = 0.0;
	t->string = NULL;
	return t;
}

void token_free (TToken* t) {
	if (t->string != NULL)
		free(t->string);
	free(t);
}


TToken* get_next (TToken* t, Tstack* s, int *storage) {       // simuluje cinost lex.analyzatoru

    const char *keywords[] = {"as", "asc", "declare", "dim", "do", "double", "else",
    "end", "chr", "function", "if", "input", "integer", "length", "loop", "print",
    "return", "scope", "string", "substr", "then", "while", "and", "boolean",
    "continue", "elseif", "exit", "false", "for", "next", "not", "or", "shared",
    "static", "true"};
    
    char c;
    char com1, com2;
    int state = 0;
    char *tmp_s = NULL;
    int signed_exp = 0;
    int done = 0;
    
    if (t == NULL) {                //FIRST USE
        t = token_init();
    }
    
    free(t->string);
    
    if (s == NULL) {
        s = stack_init();
    }
    else {
        flush_stack(s);
    }

    if (*storage == -2) {
        c = tolower(getchar());
    }
    else {
        c = *storage;
        *storage = -2;
    }

    while (isblank(c)) {        //deletes leading zeroes
        c = getchar();
    }

    if (c == '\'') {                //skips line comment
        while (getchar() != 10) {   //10 == linefeed
            continue;
        }
        getchar();                  //to get line feed
    }
    if (c == '/') {
        state = DIV;
        if ((c = tolower(getchar())) == '/') {   //skips block comment
            state = 0;
            com1 = getchar();
            while (1) {
                com2 = getchar();
                if (com1 == '/' && com2 == '/') {
                    break;
                }
                else if (com2 == EOF) {
                    state = EOF;
                    break;
                }
                else {
                    com1 = com2;
                }
            }
            c = getchar();
        }
        else {
            *storage = c;
        }
    }

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
        done = 1;
    }

    else if (c == ';') {
        state = SEMICOLON;
        done = 1;
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
        done = 1;
    }
    
    else if (c == '-') {
        state = SUB;
        done = 1;
    }

    else if (c == '*') {
        state = MUL;
        done = 1;
    }

    else if (c == '\\') {
        state = INTDIV;
        done = 1;
    }

    else if (c == 10) {
        state = EOL;
        done = 1; 
    }

    else if (c == '(') {
        state = BRACKET_L;
        done = 1;
    }

    else if (c == ')') {
        state = BRACKET_R;
        done = 1;
    }

    else if (c == EOF) {
        state = EOF;
    }

    else {
        state = SCAN_ERR;
    }

    printf("state(IN) = %d\n", state);
    

    while(done == 0 && (c = tolower(getchar())) != EOF) {

        if (isblank(c)) {
            if (state == EXCL_M) {
                state = SCAN_ERR;
                break;
            }
            if (state != STRING_V) {
                break;
            }
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
                    done = 1;
                    *storage = c;
                }
                break;
            
            case INT_V:
                if (c >= '0' && c <= '9') {
                    push(s,c);
                }
                else if (c == '.') {
                    state = FLOAT_V;
                    push(s,c);
                }
                else if (c == 'e') {
                    state = EXPONENT;
                    push(s,c);
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;
            
            case FLOAT_V:
                if (c >= '0' && c <= '9') {
                    push(s,c);
                }
                if (c == 'e') {
                    state = EXPONENT;
                    push(s,c);
                }
                else {
                    state = SCAN_ERR;
                }
                break;

            case EXPONENT:
                if (signed_exp == 0) {
                    if (c == '-') {
                        push(s,c);
                        signed_exp = 1;
                    }
                    else if (c == '+') {
                        push(s,c);
                        signed_exp = 1;
                    }
                    else if (c >= '0' && c <= '9') {
                        push(s, '+');
                        push(s, c);
                        signed_exp = 1;
                    }
                    else {
                        state = SCAN_ERR;
                    }
                }
                else if (c >= '0' && c <='9') {
                    push(s,c);
                }
                else {
                    state = SCAN_ERR;
                }       
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
                    done = 1;
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;

            case LESS:
                if (c == '=') {
                    state = LESSEQ;
                    done = 1;
                }
                else if (c == '>') {
                    state = NEQ;
                    done = 1;
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;
            
            case DIV:
                if (c == '/') {
                    state = B_COMMENT;
                }
                else {
                    *storage = c;
                }
                break;
        }
    }

    if (state == EXPONENT) {
        state = FLOAT_V;
    }

    if ((tmp_s = malloc(sizeof(char)*(s->top + 1) + 1)) == NULL) {
        exit(-1);
    }

    for (int i = 0; i <= s->top; i++) {
        tmp_s[i] = s->bottom[i];
    }

    strcat(tmp_s, "\0");

    if (state == ID) {
        for (int i = 0; i < 35; i++) {
            if ((strcmp(tmp_s, keywords[i])) == 0) {
                state = i + AS;
            }
        }
    }

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

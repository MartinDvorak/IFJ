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

int valid_ES(Tstack* s) {
    
    int c;
    int tmp = 0;

    c = tolower(getchar());
     if (c == '\\' || c == '\"' || c == 't' || c == 'n') {
        push(s, c);
    }
    else if (c >= '0' && c <= '2') {
        tmp = c - '0';
        push(s, c);
        for (int i = 0; i < 2; i++) {
            c = tolower(getchar());
            if (c >= '0' && c <= '9') {
                push(s,c);
                tmp = (tmp*10) + (c - '0');
            }
            else {
                return 0;
            }
        }
        if (tmp > 255 || tmp < 001) {
            return 0;
        }
    }
    else {
        return 0;
    }

    return 1;
}

void skip_BC (int *state) {

    int com1, com2;

    com1 = getchar();
    while (1) {
        com2 = getchar();
        if ((com1 == '\'') && (com2 == '/')) {
            break;
        }
        else if (com2 == EOF) {
        *state = SCAN_ERR;
            break;
        }
        else {
            com1 = com2;
        }
    }
}

int skip_LC(int *state) {

    int c;

    c = getchar();              //skips line comment
    while (c != 10) {           //10 == linefeed
        if (c == EOF) {
            *state = EOF;
            break;
        }
        else {
            c = getchar();
        }
    }
    return c;
}


TToken* get_next (TToken* t, Tstack* s, int *storage) {       // simuluje cinost lex.analyzatoru

    const char *keywords[] = {"as", "asc", "declare", "dim", "do", "double", "else",
    "end", "chr", "function", "if", "input", "integer", "length", "loop", "print",
    "return", "scope", "string", "substr", "then", "while", "and", "boolean",
    "continue", "elseif", "exit", "false", "for", "next", "not", "or", "shared",
    "static", "true"};
    
    int c;
    int state = 0;
    char *tmp_s = NULL;
    int signed_exp = 0;
    int done = 0;
    int dot = 0;
    int nonzero_e = 0;
    int nonzero_d = 0;
    


    if (t == NULL) {                //FIRST USE
        t = token_init();
    }

    if (s == NULL) {
        s = stack_init();
    }
    else {
        flush_stack(s);
    }

    if (t->string == NULL) {
        if ((t->string = malloc(sizeof(char)*2)) == NULL) {
            exit(99);
        }
        t->string = strcpy(t->string, "");
    }

    if (*storage == -2) {
        c = tolower(getchar());
    }
    else {
        c = *storage;
        *storage = -2;
    }

    while (isblank(c)) {        //deletes leading zeroes
        c = tolower(getchar());
    }

    if (c == '\'') {
         c = skip_LC(&state);
        /*c = getchar();              //skips line comment
        while (c != 10) {           //10 == linefeed
            if (c == EOF) {
                state = EOF;
                break;
            }
            else {
                c = getchar();
            }
        }*/
    }

    if (c == '/') {
        state = DIV;
        if ((c = tolower(getchar())) == '\'') {   //skips block comment
            state = 0;
            skip_BC(&state);
            c = tolower(getchar()); 
            while (isblank(c)) {
                c = tolower(getchar());
                if (c == '\'') {
                    c = skip_LC(&state);
                }
            }
        }
        else {
            done = 1;
            *storage = c;
        }
    }
    
    if (state == 0) { 
        if (c == '_') {
            state = ID;
            push(s,c);
        }

        else if ((c >= 'a') && (c <= 'z')) {
            state = ID;
            push(s,c);
        }
            
        else if ((c >= '0') && (c <= '9')) {
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
            done = 1;
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

        else if (c == '&') {
            state = AMP;
        }

        else {
            state = SCAN_ERR;
        }
    }

    while((done == 0) && ((c = tolower(getchar())) != EOF)) {          //while((done == 0) && ((c = tolower(getchar())) != EOF))
        //c = tolower(getchar()); //
        if (isblank(c)) {
            if (state == EXCL_M) {
                state = SCAN_ERR;
                break;
            }
            if (state != STRING_V) {
                break;
            }
        }

        if (state == SCAN_ERR) {
            break;
        }

        switch (state) {

            case ID:
                if (c == '_') {
                    push(s, c);
                }
                else if ((c >= 'a') && (c <= 'z')) {
                    push(s, c);
                }
                else if ((c >= '0') && (c <= '9')) {
                    push(s,c);
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;
            
            case INT_V:
                if ((c >= '0') && (c <= '9')) {
                    push(s,c);
                }
                else if (c == '.') {
                    state = FLOAT_V;
                    dot = 1;
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
                if ((c >= '0') && (c <= '9')) {
                    push(s,c);
                    nonzero_d = 1;
                }
                else if (c == 'e') {
                    if (nonzero_d) {
                        state = EXPONENT;
                        push(s,c);
                    }
                    else {
                        state = SCAN_ERR;
                    }
                }
                else if ((dot == 1) && (c == '.')) {
                    state = SCAN_ERR;
                }
                else {
                    done = 1;
                    *storage = c;
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
                    else if ((c >= '0') && (c <= '9')) {
                        push(s, '+');
                        push(s, c);
                        signed_exp = 1;
                        nonzero_e = 1;
                    }
                    else {
                        state = SCAN_ERR;
                        done = 1;
                        *storage = c;
                    }
                }
                else if ((c >= '0') && (c <='9')) {
                    push(s,c);
                    nonzero_e = 1;
                }
                else if (isspace(c)) {
                    done = 1;
                    *storage = c;
                }
                else {
                    done = 1;
                    *storage = c;
                    //state = SCAN_ERR;
                    /*if (!nonzero_e) {
                        state = SCAN_ERR;
                    }*/
                }
                break;
                
            case EXCL_M:
                if (c == '\"') {
                    state = STRING_V;
                    while (((c = getchar()) != '\"')) {
                        if (c == '\n') {
                            state = SCAN_ERR;
                            *storage = c;
                            break;
                        }
                        if (c <= 31) {      //primo lze zadavat pouze znak s ascii hodnotou vyss nez 31
                            state = SCAN_ERR;
                        }
                        if (c == EOF) {
                            state = SCAN_ERR;
                            break;
                        }
                        push(s, c);
                        if (c == '\\') {
                            if (valid_ES(s) == 0) {
                                state = SCAN_ERR;
                            }
                        }

                    }
                }
                else {
                    state = SCAN_ERR;
                }
                done = 1;
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

            case GREAT:
                if (c == '=') {
                    state = GREATEQ;
                    done = 1;
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;
            
            case AMP:
                if (c == 'b') {
                    state = BINARY;
                }
                else if (c == 'o') {
                    state = OCTAL;
                }
                else if (c == 'h') {
                    state = HEXA;
                }
                else {
                    state = SCAN_ERR;
                }
                break;
            
            case BINARY:
                if (c == '1' || c == '0') {
                    push(s, c);
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;
            
            case OCTAL:
                if ((c >= '0') && (c <= '7')) {
                    push(s, c);
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;

            case HEXA:
                if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f'))) {
                    push(s, c);
                }
                else {
                    done = 1;
                    *storage = c;
                }
                break;
        }
    }

    if (((nonzero_e == 0) && (state == EXPONENT)) || ((nonzero_d == 0) && (state == FLOAT_V))) {
        state = SCAN_ERR;
    }

    if (state == EXPONENT) {
        state = FLOAT_V;
    }

    if ((tmp_s = malloc(sizeof(char)*(s->top + 3))) == NULL) {
        exit(99);
    }   

    int i = 0;
    for (; i <= s->top; i++) {
        tmp_s[i] = s->bottom[i];
    }
    tmp_s[i] = '\0'; 


    if (state == ID) {
        for (int i = 0; i < 35; i++) {
            if ((strcmp(tmp_s, keywords[i])) == 0) {
                state = i + AS;
            }
        }
    }

    else if (state == INT_V) {
        char *ptr = NULL;
        t->int_v = strtol(tmp_s, &ptr, 10);
        if (ptr[0] != '\0') {
            state = SCAN_ERR;
        }
    }

    else if ((state == BINARY) || (state == OCTAL) || (state == HEXA)) {
        char *ptr = NULL;
        if (state == BINARY) {
            t->int_v = strtol(tmp_s, &ptr, 2);
        }
        else if (state == OCTAL) {
            t->int_v = strtol(tmp_s, &ptr, 8);
        }
        else {
            t->int_v = strtol(tmp_s, &ptr, 16);
        }

        if (ptr[0] != '\0') {
            state = SCAN_ERR;
        }
        
        state = INT_V;
    }

    else if (state == FLOAT_V) {
        char *ptr = NULL;
        t->float_v = strtod(tmp_s, &ptr);
        if (ptr[0] != '\0') {
            state = SCAN_ERR;
        }
    }

    if ((state == ID) || (state == STRING_V)) {
        free(t->string);
        if ((t->string = malloc(sizeof(char)*(s->top + 1) + 1)) == NULL) {
            exit(99);
        }
        strcpy(t->string, tmp_s);
    }

    t->type = state;

    free(tmp_s);
    free_stack(s);

	return t;
}

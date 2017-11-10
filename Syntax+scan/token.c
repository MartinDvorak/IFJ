#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include "stack.h"

void foo(int type)
{
    switch(type)
    {
        case AS:printf("AS ");break;
case ASC:printf("ASC ");break;
case DECLARE:printf("DECLARE ");break;
case DIM:printf("DIM ");break;
case DOUBLE:printf("DOUBLE ");break;
case ELSE:printf("ELSE ");break;
case END:printf("END ");break;
case CHR:printf("CHR ");break;
case FUNCTION:printf("FUNCTION ");break;
case INPUT:printf("INPUT ");break;
case INTEGER:printf("INTEGER ");break;
case LOOP:printf("LOOP ");break;
case PRINT:printf("PRINT ");break;
case RETURN:printf("RETURN ");break;
case SCOPE:printf("SCOPE ");break;
case STRING:printf("STRING ");break;
case SUBSTR:printf("SUBSTR ");break;
case THEN:printf("THEN ");break;  
case WHILE:printf("WHILE ");break;
case AND:printf("AND ");break;    
case BOOLEAN:printf("BOOLEAN ");break;
case CONTINUE:printf("CONTINUE ");break;
case ELSEIF:printf("ELSEIF ");break;
case EXIT:printf("EXIT ");break;
case FALSE_KW:printf("FALSE_KW ");break;
case FOR:printf("FOR ");break;
case NEXT:printf("NEXT ");break;
case NOT:printf("NOT ");break;
case OR:printf("OR ");break;
case SHARED:printf("SHARED ");break;
case STATIC:printf("STATIC ");break;
case TRUE_KW:printf("TRUE_KW ");break;
case IF:printf("IF ");break;
case ID:printf("ID ");break;
case INT_V:printf("INT_V ");break;
case FLOAT_V:printf("FLOAT_V ");break;
case STRING_V:printf("STRING_V ");break;
case ASSIGN:printf("ASSIGN ");break;
case EQ:printf("EQ ");break;
case NEQ:printf("NEQ ");break;
case LESS:printf("LESS ");break;
case GREAT:printf("GREAT ");break;
case LESSEQ:printf("LESSEQ ");break;
case GREATEQ:printf("GREATEQ ");break;
case ADD:printf("ADD ");break;
case SUB:printf("SUB ");break;
case MUL:printf("MUL ");break;
case DIV:printf("DIV ");break;
case INTDIV:printf("INTDIV ");break;
case DO:printf("DO ");break;
case EOL:printf("EOL ");break;
case BRACKET_L:printf("BRACKET_L ");break;
case BRACKET_R:printf("BRACKET_R ");break;
//case EOF      69  // end of file
case COLON:printf("COLON ");break; 
case SEMICOLON:printf("SEMICOLON ");break;
case EOF:printf("EOF");break;
//my preciouses
case EXCL_M:printf("EXCL_M ");break;
case EXPONENT:printf("EXPONENT ");break;
case L_COMMENT:printf("L_COMMENT ");break;
case B_COMMENT:printf("B_COMMENT ");break;
case SCAN_ERR:printf("SCAN_ERR ");break;
case LENGTH:printf("LENGTH ");break;

    }
}


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
    
    int c;
    char com1, com2;
    int state = 0;
    char *tmp_s = NULL;
    int signed_exp = 0;
    int done = 0;
    int dot = 0;
    int nonzero = 0;

    
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
        c = getchar();              //skips line comment
        while (c != 10) {           //10 == linefeed
            if (c == EOF) {
                state = EOF;
                break;
            }
            else {
                c = getchar();
            }
        }
      //  getchar();                  //to get line feed
    }
    if (c == '/') {
        state = DIV;
        if ((c = tolower(getchar())) == '\'') {   //skips block comment
            state = 0;
            com1 = getchar();
            while (1) {
                com2 = getchar();
                if ((com1 == '\'') && (com2 == '/')) {
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
            c = tolower(getchar());
            while (isblank(c)){
                c = tolower(getchar());
            }
        }
        else {
            done = 1;
            *storage = c;
        }
    }

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
    
    while((done == 0) && ((c = tolower(getchar())) != EOF)) {
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
                }
                else if (c == 'e') {
                    state = EXPONENT;
                    push(s,c);
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
                        nonzero = 1;
                    }
                    else {
                        state = SCAN_ERR;
                    }
                }
                else if ((c >= '0') && (c <='9')) {
                    push(s,c);
                    nonzero = 1;
                }
                else {
                    if (!nonzero) {
                        state = SCAN_ERR;
                    }
                    else {
                        done = 1;
                        *storage = c;
                    }   
                }
                break;
            case EXCL_M:
                if (c == '\"') {
                    state = STRING_V;
                    while (((c = tolower(getchar())) != '\"') && (c != '\n')) {
                        push(s, c);
                        /*if (c == '\\') {
                            c = tolower(getchar());
                            if (c != '\\' && c != '\"' && c != 't' && c != 'n') {
                            }
                        }*/

                    }
                    //c = getchar();
                   // break;
                }
                else {
                    state = SCAN_ERR;
                }
                done = 1;
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

    if ((tmp_s = malloc(sizeof(char)*(s->top + 3))) == NULL) {
        exit(99);
    }   

    int i = 0;
    for (; i <= s->top; i++) {
        tmp_s[i] = s->bottom[i];
    }
    tmp_s[i] = '\0'; 

    //printf("%s\n", tmp_s);
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

    if (state == FLOAT_V) {
        char *ptr = NULL;
        t->float_v = strtod(tmp_s,&ptr);
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

    free(tmp_s);
    free_stack(s);

    foo(t->type);

    return t;
}
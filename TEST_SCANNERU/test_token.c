#include <stdio.h>
#include <stdlib.h>
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

    }


}


int main(int argc, char **argv)
{
    TToken* token = NULL;
    Tstack* s = NULL;
    int storage = -2;
    int i = 1; 

    while(1) {
        token = get_next(token, s, &storage);
        foo(token->type);
        if (token->type==EOL)
            printf("\n");
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
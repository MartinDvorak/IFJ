#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define AS          10  //10 - 44 keywords
#define ASC         11
#define DECLARE     12
#define DIM         13
#define DO          14
#define FLOAT       15
#define ELSE        16
#define END         17
#define CHR         18
#define FUNCTION    19
#define IF          20
#define INPUT       21
#define INTEGER     22
#define LENGTH      23
#define LOOP        24
#define PRINT       25
#define RETURN      26
#define SCOPE       27
#define STRING      28
#define SUBSTR      29
#define THEN        30
#define WHILE       31
#define AND         32
#define BOOLEAN     33
#define CONTINUE    34
#define ELSEIF      35
#define EXIT        36
#define FALSE_KW    37
#define FOR         38
#define NEXT        39
#define NOT         40
#define OR          41
#define SHARED      42
#define STATIC      43
#define TRUE_KW     44

#define ID          50   //identifikátor
#define INT_V       51
#define FLOAT_V     52
#define STRING_V    53
#define ASSIGN      54  //=
#define EQ          55  //==
#define NEQ         56  //<>
#define LESS        57  //<
#define GREAT       58  //>
#define LESSEQ      59  //<=
#define GREATEQ     60  //>=
#define ADD         61  //+
#define SUB         62  //-
#define MUL         63  //*
#define DIV         64  //"/"
#define INTDIV      65  //"\"

#define EOL			66  // '\n' - end of line
#define BRACKET_L   67  // (
#define BRACKET_R	68	// )
//#define EOF 		69  // end of file
#define COLON		70  // , 
#define SEMICOLON	69  // ;
//#define EOF -1
//my preciouses
#define EXCL_M		71	// !
#define EXPONENT	72  //float value with exponent
#define L_COMMENT	89  //in line comment
#define B_COMMENT	90	//in block commnet
#define SCAN_ERR	666 //lexem error
void foo(int type)
{
	switch(type)
	{
		case AS:printf("AS ");break;
case ASC:printf("ASC ");break;
case DECLARE:printf("DECLARE ");break;
case DIM:printf("DIM ");break;
case FLOAT:printf("FLOAT ");break;
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
//case EOF 		69  // end of file
case COLON:printf("COLON ");break; 
case SEMICOLON:printf("SEMICOLON ");break;
//case EOF -1
//my preciouses
case EXCL_M:printf("EXCL_M ");break;
case EXPONENT:printf("EXPONENT ");break;
case L_COMMENT:printf("L_COMMENT ");break;
case B_COMMENT:printf("B_COMMENT ");break;
case SCAN_ERR:printf("SCAN_ERR ");break;

	}


}

int main()
{
	return 0;
}
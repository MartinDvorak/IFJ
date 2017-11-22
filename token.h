#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define TRUE        1
#define FALSE       0

#define AS          10  //10 - 44 keywords
#define ASC         11
#define DECLARE     12
#define DIM         13
#define DO          14
#define DOUBLE      15
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

#define ID          50   //identificator
#define INT_V       51
#define FLOAT_V   	52
#define STRING_V    53
#define ASSIGN      54  //=
//#define EQ          55  //==
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
#define EOL			66	//'\n' - end of line
#define BRACKET_L	67  // (
#define BRACKET_R 	68  // )
#define SEMICOLON	69 	// ;
#define COLON		70  // ,
//#define EOF -1
//my preciouses
#define EXCL_M		71	// !
#define EXPONENT	72  //float value with exponent
#define L_COMMENT	89  //in line comment
#define B_COMMENT	90	//in block commnet
#define SCAN_ERR	666 //lexem error


#ifndef TTOKEN
typedef struct token{
	int type;
	
	int int_v;
	double float_v;
    char *string;

}TToken;
#endif
#define TTOKEN

TToken* token_init();

void token_free(TToken* t);

TToken *get_next(TToken *t, Tstack* s, int *storage);

int valid_ES(Tstack* s);

//vraci posledni nacteny znak tedy EOL
int skip_LC(int *state);

void skip_BC(int *state);
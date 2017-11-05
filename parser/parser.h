#include <stdio.h>
#include <stdlib.h>

#define TRUE        1
#define FALSE       0

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

#define BODY		100

#ifndef TTOKEN
typedef struct token{
	int type;
	
	int int_v;
	float float_v;
    char *string;

}TToken;
#endif
#define TTOKEN

#ifndef TTREE
typedef struct data{
	int type;
	int declared;
	char* param; 

}Tdata;

typedef struct tnode{
	char* key; // name 
	Tdata data; 	
	
	struct tnode* lptr;
	struct tnode* rptr;

}*Ttnode_ptr;
#endif
#define TTREE

int comapare(TToken *t, int type);

TToken *give_me(TToken *t);

int scope(TToken *t);

int func_line(TToken *t, int declared);

int func(TToken *t);

int param(TToken *t, Tdata* data );

// 0 - param, 1 - type
int type(TToken *t, Tdata* data, int type_or_param);

int parser_FREEBASIC(TToken *t);

TToken* give_me(TToken* t);
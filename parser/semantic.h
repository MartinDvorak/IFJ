#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbtab.h"


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
	int defined;
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

// vloze pouze build in fce do stromu globalniho
void semantic_insert_build_in();

// prevede char na #define hodnotu 'i' -> INTEGER
int semantic_convert_data_type (char c);

// promenou v tokenu vezme najde ve stromu - neni err
// a pote porovna s datovym typev c char data_type
// indikuje implicitni konverze
int semantic_id(Ttnode_ptr root, TToken* t, char data_type);

// stejne jako semantic_id akorat vstup je string param a pohybuje se pomoci position
int semantic_id_param(TToken *t, char* param, int* position);

// nalezne jestli existuje fce a tomom vrátí jeji parametry v  char*param
int semantic_fce_param(Ttnode_ptr root, TToken* t, char* param);


int semantic_insert(Ttnode_ptr* root, char* name, Tdata* data);

int semantic_find_id(TToken* t);

int semantic_call_undefined_fce();
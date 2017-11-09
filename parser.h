#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "symbtab.h"

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
#define SEMICOLON	71  // ;



typedef struct semantic_operation{
	char op;  // what is operation
	int l_convert; // left value need convert
	int r_convert; // right valur need convert
}Toperation;

TToken* give_me(TToken* t);// simuluje cinost lex.analyzatoru

int scope(TToken *t);

int func_line(TToken *t, int declared);

int func(TToken *t);

// 0 - param, 1 - type
int type(TToken *t, Tdata* data, int type_or_param, int* to_symbtab);

int parser_FREEBASIC(TToken *t);

void add_char_to_param(TToken *t, Tdata *data);

int body(TToken *t);

// local 1 = insert in local tree
// pro definice funkci
int params_N(TToken *t, Tdata *data, int local);

int param(TToken *t, Tdata* data, int local );

// pro kontrolu volani fci
int param_f(TToken *t, char* string, int* position);

int param_fn(TToken *t, char* string, int* position);

int preprocesing_expr(TToken *t,TToken *last,int condition,int* exp_ret);

int equal(TToken *t, int l_value);

int expr_n(TToken *t);

int r_side(TToken *t,int l_value);

int build_in_fce(TToken *t);



////////////////////////////////////////////////////////////////////////////
// 						SEMANTIC

// vloze pouze build in fce do stromu globalniho
void semantic_insert_build_in();

// convertuje type_vlaue na type
int semantic_id_type_covert(int type);

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

// vlozi pokud neexistuje jinak false
int semantic_insert(Ttnode_ptr* root, char* name, Tdata* data);

// false neexistuje, v typu bude hodnota type promene
int semantic_id_type(Ttnode_ptr root,TToken* t,int* type );

// local find id
int semantic_find_id(TToken* t);

// kouka jeslti jsou nejake funkce ktere nejsou definovane ale jsou volane (volane s deklaraci)
int semantic_call_undefined_fce();

// string - postfix notace, type array - dat. typy promennych,, arr - pole struct promennych, num_of_arr - pocet vyslednych prvku
// zkontroluje dat type a pretypuje popripade promenne
int semantic_exp(char* string, int* type_array, Toperation* arr, int* num_of_arr, int* exp_ret);

int semantic_check_lside_rside(int l_side, int r_side);

int semantic_id_type_convert(int type);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbtab.h"
#include "token.h"
#include "parser.h"


#ifndef OPSTR
typedef struct semantic_operation{
	char op;  // what is operation
	int l_convert; // left value need convert
	int r_convert; // right valur need convert
}Toperation;
#endif
#define OPSTR

#ifndef OPERAND_STR
typedef struct expr_operand{

	int semantic_type;			//pro semantickou kontrolu, bez rozdilu pro konst. a promenne
	int type;
	char* name;
	int int_v;
	float float_v;
	char* string;
}TExpr_operand;
#endif
#define OPERAND_STR

// vloze pouze build in fce do stromu globalniho
void semantic_insert_build_in();

// convertuje type_vlaue na type
int semantic_id_type_convert(TToken* t);

// prevede char na #define hodnotu 'i' -> INTEGER
int semantic_convert_data_type (char c);

// promenou v tokenu vezme najde ve stromu - neni err
// a pote porovna s datovym typev c char data_type
// indikuje implicitni konverze
int semantic_id(Ttnode_ptr root, TToken* t, char data_type, int* convert_param);

// stejne jako semantic_id akorat vstup je string param a pohybuje se pomoci position
int semantic_id_param(TToken *t, char* param, int* position, int* convert_param);

// nalezne jestli existuje fce a tomom vrátí jeji parametry v  char*param
int semantic_fce_param(Ttnode_ptr root, TToken* t, char** param);

// vlozi pokud neexistuje jinak false
int semantic_insert(Ttnode_ptr* root, char* name, Tdata* data);

// vlozi a podiva se do druheho stromu jejistli tam neni chova se najko semantic_insert
int semantic_insert_id(Ttnode_ptr* root,Ttnode_ptr root_check, char* name, Tdata* data);

// false neexistuje, v typu bude hodnota type promene
int semantic_id_type(Ttnode_ptr root,TToken* t,int* type );

// local find id
int semantic_find_id(TToken* t);

// kouka jeslti jsou nejake funkce ktere nejsou definovane ale jsou volane (volane s deklaraci)
int semantic_call_undefined_fce();

// string - postfix notace, type array - dat. typy promennych,, arr - pole struct promennych, num_of_arr - pocet vyslednych prvku
// zkontroluje dat type a pretypuje popripade promenne
int semantic_exp(char* string, TExpr_operand* operand_array, Toperation* arr, int* num_of_arr, int* exp_ret);

int semantico_convert_buildin(int type);

int semantic_check_lside_rside(int l_side, int r_side, int r_side_type, int* convert_func_result);

int semantic_return_type(int* glob_var,int local,int ret_type, char* name, int flag);

int semantic_check_define(Ttnode_ptr* root, char* name);

void semantic_flag_use(Ttnode_ptr* root,TToken* t);

int semantic_check_params(Ttnode_ptr root,char* name, char* param);

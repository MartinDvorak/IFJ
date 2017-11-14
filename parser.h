#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "symbtab.h"
#include "stack.h"
#include "expr.h"
#include "token.h"

#define TRUE        1
#define FALSE       0

// GLOBAL VARIABLES FOR symbtab
//*************************************************

#ifndef ST_GV
#define ST_GV

/*
Ttnode_ptr root_global = NULL;
Ttnode_ptr root_local = NULL;
int return_type = 0;
*/

extern Ttnode_ptr root_global;
extern Ttnode_ptr root_local;
extern int return_type;

#endif

//***********************************************
// GLOBAL for lex. analyzator
//***********************************************
#ifndef LA_GV
#define LA_GV
/*
Tstack* LA_S = NULL;
int storage = -2;
*/

extern Tstack* LA_S;
extern int storage;
#endif

//***********************************************
// GLOBAL for syntax errors
//***********************************************
#ifndef ERR_GV
#define ERR_GV
//int ERROR_RETURN = 2; //syntax error

extern int ERROR_RETURN;

#endif

//***********************************************


TToken* give_me(TToken* t);// simuluje cinost lex.analyzatoru

int scope(TToken *t);

int func_line(TToken *t, int declared);

// scope = FALSE -> nebyl jeste, TRUE -> uz byl 
int func(TToken *t, int scope);

// 0 - param, 1 - type
int type(TToken *t, Tdata* data, int type_or_param, int* to_symbtab);

int parser_start(TToken *t);

int parser_FREEBASIC();

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


#endif //PARSER_H_
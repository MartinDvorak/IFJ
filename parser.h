
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbtab.h"
#include "stack.h"
#include "expr.h"
#include "token.h"

#define TRUE        1
#define FALSE       0

// GLOBAL VARIABLES FOR symbtab
//*************************************************
#ifndef ST_GV
Ttnode_ptr root_global = NULL;
Ttnode_ptr root_local = NULL;
int ERROR_RETURN = 2; //syntax error
#endif
#define ST_GV
//***********************************************
// GLOBAL for lex. analyzator
//***********************************************
#ifndef LA_GV
Tstack* LA_S = NULL;
int storage = -2;
#endif
#define LA_GV
//***********************************************


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
#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbtab.h"
#include "stack.h"
#include "expr.h"
#include "token.h"
#include "codegen.h"

#define TRUE        1
#define FALSE       0

// GLOBAL VARIABLES FOR symbtab
//*************************************************

#ifndef ST_GV
#define ST_GV

extern Ttnode_ptr root_global;
extern Ttnode_ptr root_local;
extern int return_type;

#endif

//***********************************************
// GLOBAL for lex. analyzator
//***********************************************
#ifndef LA_GV
#define LA_GV

extern Tstack* LA_S;
extern int storage;

#endif

//***********************************************
// GLOBAL for syntax errors
//***********************************************
#ifndef ERR_GV
#define ERR_GV

extern int ERROR_RETURN;

#endif

//***********************************************



/**
 * @brief      reprezentuje neterminál SCOPE
 *
 * @param      t     ukazatel na aktuální token
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int scope(TToken *t);



/**
 * @brief      reprezetnuje neterminál FUNC_LINE
 *
 * @param      t         ukazatel na aktuální token
 * @param[in]  declared  příznak zda je funkce delkaroána nebo definována
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int func_line(TToken *t, int declared);



/**
 * @brief      reprezentuje neterminál F
 *
 * @param      t      ukazatel na aktuální token
 * @param[in]  scope  určuje zda jsme již neterminál SCOPE či nikoliv
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int func(TToken *t, int scope);



/**
 * @brief      ukládá datový typ do proměné data
 *
 * @param      t              ukazatel na aktuální token
 * @param      data           ukazatel na data, do kterých se bude ukládat
 * @param[in]  type_or_param  určuje zda se jedná u type fce nebo proměnné, zda
 *                            budeme ukládat do paramentů nebo jako dat.typ
 * @param      to_symbtab     TODO
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int type(TToken *t, Tdata* data, int type_or_param, int* to_symbtab);



/**
 * @brief      reprezentuje neterminál S (start)
 *
 * @param      t     ukazatel na aktuální token
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int parser_start(TToken *t);



/**
 * @brief      uvodní funkce, při zpustění překladače, inicilizuje potřebné
 *             proměné a následně je po překladu uvolnuje
 *
 * @return     vrací error výstupy, podle předem daného zadání
 */
int parser_FREEBASIC();



/**
 * @brief      ukldádá do data.param další parametr dané funkce
 *
 * @param      t     ukazatel na aktuální token
 * @param      data  ukazatel na data do kterých se bude ukládat další parametr
 */
void add_char_to_param(TToken *t, Tdata *data);



/**
 * @brief      reprezentuje neterminál BODY
 *
 * @param      t     ukazatel na aktuální token
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int body(TToken *t);

// local 1 = insert in local tree
// pro definice funkci

/**
 * @brief      reprezentuje neterminál PARAMS_N
 *
 * @param      t      ukazatel na aktuální token
 * @param      data   ukazatel na data, do kterých se ukládají načtené
 *                    paramentry
 * @param[in]  local  pokud je nastaven ukládáme do lokální tabulky symbolů jako
 *                    paramentry
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE

 */
int params_N(TToken *t, Tdata *data, int local);


/**
 * @brief      reprezentuje neterminál PARAMS
 *
 * @param      t      ukazatel na aktuální token
 * @param      data   ukazatel na data, do kterých se ukládají načtené
 *                    paramentry
 * @param[in]  local  pokud je nastaven ukládáme do lokální tabulky symbolů jako
 *                    paramentry
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int param(TToken *t, Tdata* data, int local );



/**
 * @brief      reprezentuje neterminál PARAM_F
 *
 * @param      t         ukazatel na aktuální token
 * @param      string    ukazatel na pole paramenetrů pro semantickou kontrolu
 * @param      position  určije jaký dat. typ parametru má semantika očekávat
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int param_f(TToken *t, char* string, int* position);


/**
 * @brief      reprezentuje neterminál PARAM
 *
 * @param      t         ukazatel na aktuální token
 * @param      string    ukazatel na pole paramenetrů pro semantickou kontrolu
 * @param      position  určije jaký dat. typ parametru má semantika očekávat
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání pomocí LL tabulky, jinak
 *             FALSE
 */
int param_fn(TToken *t, char* string, int* position);



/**
 * @brief      NEreperezentuje žádný neterminál, funkce složí jako
 *             předzpracování výrazu pro přepnutí se do precedenčního zpracování
 *             výrazů. upravuje celý výraz aby bylo jendodušší následné jeho
 *             zpracování
 *
 * @param      t          ukazatel na aktuální token
 * @param[in]  condition  volí se jiná tabulka pokud jsme ve statementu IF nebo
 *                        WHILE.
 * @param      exp_ret    datový typ celého výrazu
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání, jinak
 *             FALSE
 */
int preprocesing_expr(TToken *t,int condition,int* exp_ret);



/**
 * @brief      reprezunetuje neterminál =
 *
 * @param      t                    ukazatel na aktuální token
 * @param[in]  lvalue               dat. typ hondoty na levé straně před
 *                                  přiřazením
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání, jinak
 *             FALSE
 */
int equal(TToken *t,int lvalue, char* name);



/**
 * @brief      reprezentuje neterminál EXṔ_N 
 *
 * @param      t     ukazatel na aktuální token
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání, jinak
 *             FALSE
 */
int expr_n(TToken *t);



/**
 * @brief      Nereprezentuje neterminál. pomocná fce, která je pro zjednodušení
 *             kontroly semantiky.
 *
 * @param      t                    ukazatel na aktuální token
 * @param[in]  lvalue               dat.typ výrazu na levé straně
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání, jinak FALSE
 */
int r_side(TToken *t,int lvalue, char* name);



/**
 * @brief      NEreprezentuje žádná neterminál. pomocná funkce, který ve
 *             zpracování výrazů rozeznává od sebe ID a ID(PARAMS)
 *
 * @param      t                     ukazatel na aktuální token
 * @param      type_id               ukazatel na dat. typ, který bude nastaven
 *                                   podle přijatého ID
 * @param      operand_array         pole operandu
 * @param      ptr_to_array          prvni volna pozice v poli operandů
 * @param      calls_per_expression  počet volání funkcí v rámci jednoho výrazu
 *
 * @return     TRUE = proběhlo úspěšně zpracovnání, jinak FALSE
 */
int look_ahead(TToken *t, int* type_id, TExpr_operand* operand_array, int* ptr_to_array, int* calls_per_expression);

#endif //PARSER_H_
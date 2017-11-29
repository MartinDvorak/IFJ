#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define TRUE 1
#define FALSE 0


/**
 * @brief      redukuje terminály na tenerminály na zásobníku
 *
 * @param      s         ukazatel na zásobník, kde se pracuje
 * @param      postfix   ukazatel na výstupní postfixový řetezec, který je
 *                       tvořen
 * @param      position  ukazatel na první místo v řetezci postfix
 *
 * @return     vrací TRUE pokud se akce povedla == exituje pravidlo na zpracovná
 *             dat, které jsou na zásobníku. Jinak FALSE
 */
int reduce(Tstack*s, char* postfix, int* position);




/**
 * @brief      vrací hodnotu tabulky, která je adresována parametry
 *
 * @param      table         ukazatel na tabulku
 * @param[in]  token_symbol  adresa řádku
 * @param[in]  stack_symbol  adresa sloupce
 * @param[in]  conditin      rozdílné zda řešíme výraz v podmíněném příkazu if
 *                           nebo while
 *
 * @return     vrací hodnotu z tabulky, která byla naadresována
 */
char table_precedence(char** table,char token_symbol, char stack_symbol, int conditin);



/**
 * @brief      původní inicializace a naplnění hodnotami do tabulky
 *
 * @param[in]  conditin  říká jakou verzi tabulky budeme inicializovat candition
 *                       = true, potom inicializujeme tabulku pro podmíněný
 *                       příkaz
 *
 * @return     vrací ukazatel na tabulku
 */
char **init_table_precedence(int conditin);



/**
 * @brief      uvolnuje tabulku v paměti
 *
 * @param      table     ukazatel na tabulku
 * @param[in]  conditin  udává napřímo velikost tabulky
 */
void free_table(char **table, int conditin);




/**
 * @brief      hlavni funkce modulu expr.c, funkce je implementací algoritmu
 *             precedenčního zpracování výrazů. viz dokumentace.pdf
 *
 * @param      string     vstupní parametr s připravenými tokeny
 * @param[in]  condition  rozhoduje jaký sénář nastane, jaká se uplatní tabulka
 * @param      postfix    ukazatel na výstupní řetezec postixového výsledku
 *
 * @return     TRUE pokud proběhlo vše v pořádku, jinak FALSE
 */
int expr(char* string, int condition, char* postfix);



/**
 * @brief      pracuje se vstupním řetezcem
 *
 * @param      string    ukazatel na vstupní řetezec
 * @param      position  ukazatel na aktuální znak, který má být vrácen
 *
 * @return     znak v řetezci string na pozici position
 */
char next(char* string, int *position);

#endif
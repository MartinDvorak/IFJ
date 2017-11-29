#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

#ifndef SSTACK
typedef struct stack{
	int* bottom;
	int top;
	int max;
}Tstack;
#endif
#define SSTACK

/**
 * @brief      realukuje pomocí fce realloc veliskost zásobníku pokud nastane
 *             situace, že je plný a je třeba přidat další prvek
 *
 * @param      s     ukazatel na zasobník
 */
void stack_realloc(Tstack* s);


/**
 * @brief      původní inicializayce zásobníku před použitím
 *
 * @return     vrací ukazatel na inicializovaný zásobník
 */
Tstack *stack_init();


/**
 * @brief      akce pop na zásobníku
 *
 * @param      s     ukazatel na zásobník
 *
 * @return     vrací vrchol zásobníku
 */
int pop(Tstack* s);


/**
 * @brief      nalezne prvni terminál na zásovníku
 *
 * @param      s     ukazatel na zásobník
 *
 * @return     vrací hodnotu nalezeného terminálu
 */
int top_stack_terminal(Tstack* s);



/**
 * @brief      uloží na požadovanou pozici podle alg. znak '<'
 *
 * @param      s     ukazatel na zásobník
 */
void stack_push_lt(Tstack* s);



/**
 * @brief      vypisuje zásobník
 *
 * @param      s     ukazatel na zsásobník
 */
void printstack(Tstack* s);



/**
 * @brief      ukládá na vrchol zásobníku
 *
 * @param      s       ukazatel na zásovník
 * @param[in]  insert  data, které se mají ukládat na vrchol
 */
void push(Tstack*s, int insert);



/**
 * @brief      uvolnění paměti, která byla alokovaná pro zásobník
 *
 * @param      s     ukazatel na zásobník
 */
void free_stack(Tstack*s);



/**
 * @brief      čte vrchol, a nechává ho stále na zásobníku
 *
 * @param      s     ukazatel na zásobník
 *
 * @return     hodnotu vrcholu zásobníku, pokud je prázdný tak -1
 */
int top_stack(Tstack* s);



/**
 * @brief      přečte vrchol zásobníku zásobníku
 *
 * @param      s     ukazatel na zásobník
 *
 * @return     vrací vrchol zásovníků
 */
int read_top_pop(Tstack* s);




/**
 * @brief      zjištuje zda je zásobník prázdný
 *
 * @param      s     ukazatel na zásobník
 *
 * @return     vrací bool true = empty,
 */
int stack_empty(Tstack* s);



/**
 * @brief      pracuje s prvkem, který je za vrcholem
 *
 * @param      s     ukazatel na zásobník
 *
 * @return     prvek, který se nachází zavrcholem
 */
int top2_stack(Tstack* s);



/**
 * @brief      obdobná operace jako int pop(Tstack *s); pouze popne dva prvky
 *
 * @param      s     ukazatel na zásobník
 */
void pop2(Tstack*s);



/**
 * @brief      vyprázní celý zásobník
 *
 * @param      s     ukazatel na zásobník
 */
void flush_stack(Tstack* s);

#endif
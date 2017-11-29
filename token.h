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
#define NEQ         55  //<>
#define LESS        56  //<
#define GREAT       57  //>
#define LESSEQ      58  //<=
#define GREATEQ     59  //>=
#define ADD         60  //+
#define SUB         61  //-
#define MUL         62  //*
#define DIV         63  //"/"
#define INTDIV      64  //"\"
#define EOL			65	//'\n' - end of line
#define BRACKET_L	66  // (
#define BRACKET_R 	67  // )
#define SEMICOLON	68 	// ;
#define COLON		69  // ,
//#define EOF -1
//my preciouses
#define EXCL_M		70	// !
#define EXPONENT	71  //float value with exponent
#define SCAN_ERR	666 //lexem error

//expansion
#define AMP			72  //&
#define BINARY		73	//int in binary
#define	OCTAL		74  //int in octal
#define HEXA		75	//int in hexadecimal


#ifndef TTOKEN

/**
 * @brief Struktura reprezentujici jeden token.
 */

typedef struct token{
	int type;		/**< Urcuje typ tokenu.*/
	
	int int_v;		/**< Pokud je token typu integer, hodnota se ulozi do int_v.*/
	double float_v;	/**< Pokud je token typu double, hodnota se ulozi do float_v.*/
    char *string;	/**< Pokud je token typu id nebo string , hodnota se ulozi do *string.*/

}TToken;
#endif
#define TTOKEN

/**
 * @brief Alokuje misto pro token a inicalizuje jeho cleny.
 * 
 * @return Vraci adresu na inicializovany token.
 */ 
TToken* token_init();

/**
 * @brief Smaze token.
 * 
 * Pokud je clen tokenu *string ruzny od NULL uvolni alokovanou pamet. Pote uvolni pamet pro
 * celou celou strukturu token.
 * 
 * @param t Ukazatel na token, ktery bude uvolnen.
 */
void token_free(TToken* t);

/**
 * @brief Prochazi zdrojovym souborem a vrati dalsi token.
 * 
 * TODO
 * 
 * @param t Ukazatel na token, 
 * @param s Zasobnik na uschovu znaku, pro nasledne ulozeni do string hodnoty tokenu.
 * @param storage Ukazatel na integer, ve kterem je uschovan znak z predesleho volani funkce, ktery byl precten, ale nezpracovan.
 * 
 * @return Vraci ukazatel s aktualizovanym tokenem.
 */
TToken *get_next(TToken *t, Tstack* s, int *storage);

/**
 * @brief Vyhodnocuje zda je zadana escape sekvence uvnitr retezce validni.
 * 
 * @param s Zasobnik, na ktery bude ulozena validni escape sekvence.
 * 
 * @return Vraci jednicku pokud je escape sekvence validni a nulu v opacnem pripade.
 */
int valid_ES(Tstack* s);

/**
 * @brief Preskoci radkovy komentar.
 * 
 * @param state Pokud narazi na EOF, tak zmeni stav na EOF.
 * 
 * @return Vraci posledni nacteny znak, tedy LF.
 */
int skip_LC(int *state);

/**
 * @brief Preskoci blokovy komentar.
 * 
 * @param state Pokud je komentar spatne zadany meni @p state na SCAN_ERR.
 */
void skip_BC(int *state);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbtab.h"
#include "token.h"
#include "parser.h"


/**
 * Struktura popisující jeden operátor/operaci ve výrazu. Společně s polem
 * operandů (viz. níže) reprezentuje výraz.
 */
#ifndef OPSTR
typedef struct semantic_operation{
	char op;  // operátor
	int l_convert; // příznak nutnosti přetypování levé strany
	int r_convert; // příznak nutnosti přetypování pravé strany
}Toperation;
#endif
#define OPSTR


/**
 * Struktura popisující jeden operand výrazu (nebo také návratovou hodnotu
 * funkce volané ve výrazu). Společně s polem operátorů/operací (viz. výše)
 * reprezentuje výraz.
 */
#ifndef OPERAND_STR
typedef struct expr_operand{

	int semantic_type;	//typ pro semantickou kontrolu, bez rozdilu pro konst. a promenne
	int type;	//typ pro generování kódu
	int is_return_value;	//TRUE - jedná se návrat. hodnotu funkce, FALSE - proměnná
	char* name;	 	//identifikátor 
	int int_v;			
	float float_v;
	char* string;
}TExpr_operand;
#endif
#define OPERAND_STR

/**
 * @brief      vlkádá build_in funkce do globálníhí tabulky symbolů
 */
void semantic_insert_build_in();


/**
 * @brief      ukládá jako stejné hodnoty proměnné a konstatny tzn. INTEGER =
 *             INT_V
 *
 * @param[in]  type     jaký datový typ má být převeden
 * @param[in]  type_id  pokud je zadáno id, je vráceno id
 *
 * @return     dat.typ daného prvnku, pokud selže tak vrací 0
 */
int semantic_id_type_convert(int type,int type_id);


/**
 * @brief      přetváří znak v řetezci parametrů funkce na dat. typ
 *
 * @param[in]  c     parametr, který se bude převádět
 *
 * @return     int hodnotu dat. typ
 */
int semantic_convert_data_type (char c);


/**
 * @brief      zjištujě zda dat. typy odpovídají definici popřípadě implicitní
 *             konverzi
 *
 * @param[in]  root           ukazatel na kořen tabulky symbolů, se kterou se
 *                            bude pracovat
 * @param      t              ukazatel na aktuální token = získaný dat. typ
 * @param[in]  data_type      očekáváný dat. typ
 * @param      convert_param  TODO
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací
 *             FALSE
 */
int semantic_id(Ttnode_ptr root, TToken* t, char data_type, int* convert_param);


/**
 * @brief      získává z tabulky symbolů parametry aktuální funkce
 *
 * @param[in]  root   ukazatel na kořen tabulky symbolů
 * @param      t      ukazatel na aktuální token
 * @param      param  ukazatel na string, kde mají být uložený získané parametry
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací
 *             FALSE
 */
int semantic_fce_param(Ttnode_ptr root, TToken* t, char** param);


/**
 * @brief      vkládá pořadovaný klíč a data do tabulky symbolů
 *
 * @param      root  ukazatel na kořen tabuluky symbolů
 * @param      name  klíč podlé, kterého se bude vkládat
 * @param      data  data, která se mají vložit
 *
 * @return     TRUE pokud vložení uzlu dopadlo v pořádku, jinak vrací FALSE
 */
int semantic_insert(Ttnode_ptr* root, char* name, Tdata* data);


/**
 * @brief      obodná funkce k semantic_insert. rozdíl spočívá v tom, že se
 *             kontrolůje ještě druhá tabulka symbolů, kvůli refedinici
 *
 * @param      root        ukazatel na kořen, kde se bude vkládat id
 * @param[in]  root_check  ukazatel na kořen, který se bude kontrolovat zda již
 *                         takové id neexistuje
 * @param      name        klíč, podle kterého se bude vyhledávat
 * @param      data        data, která se mají vložit
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku a uzel je vložen, jinak vrací FALSE
 */
int semantic_insert_id(Ttnode_ptr* root,Ttnode_ptr root_check, char* name, Tdata* data);


/**
 * @brief      zjištujě a ukládá do proměné dat. typ ID, který j uložen v tokenu
 *
 * @param[in]  root  ukazatel na tabulku symbolů, která se bude porhledávat
 * @param      t     ukazatel na aktuální token
 * @param      type  ukazatel na proměnnou kám se má uložit nalezený dat. typ
 *
 * @return     TRUE pokud nalezneme prvek, jinak vrací FALSE
 */
int semantic_id_type(Ttnode_ptr root,TToken* t,int* type );



/**
 * @brief      kontroluje zda je dané ID zadefinováno
 *
 * @param      t     ukazatel na aktuální token, ID, který je zkoumaný
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací
 *             FALSE
 */
int semantic_find_id(TToken* t);


/**
 * @brief      procházi globální tabulku symbolů a kontroluje zda byla nějaká
 *             funkce declarována  volána ale zaroveň nedefinována
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací FALSE
 */
int semantic_call_undefined_fce();

// string - postfix notace, type array - dat. typy promennych,, arr - pole struct promennych, num_of_arr - pocet vyslednych prvku
// zkontroluje dat type a pretypuje popripade promenne
// 

/**
 * @brief      semantická kontrola celého výrazu. Výraz je kontrolovaný pomocí
 *             postfixového formátu, při čemž se pracuje se zásobníkem
 *
 * @param      string         řetezec v postixovém formátu
 * @param      operand_array  pole operandů, obsahující také datový typ operandu
 * @param      arr            ukazatel na strukturu, kdeje uloženo znaménko a
 *                            přiznaky zda se má jeden z operátozu pomocí
 *                            implicitní konverze upravit
 * @param      num_of_arr     ukazatel na první volný prvek ke struktuře arr
 * @param      exp_ret        ukazatel, který má být naplněn výstupní hodnotou
 *                            celého výrazu
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací FALSE
 */
int semantic_exp(char* string, TExpr_operand* operand_array, Toperation* arr, int* num_of_arr, int* exp_ret);



/**
 * @brief      ujištuje návratové typy build_in funkcí
 *
 * @param[in]  type  položka v aktuálním tokenu, která říká jaká build_in fce je
 *                   zpracovávaná
 *
 * @return     vrací dat. typ build_in funkce
 */
int semantic_convert_buildin(int type);



/**
 * @brief      kontroluje leovu stranu výrazu a pravou stranu výrazu
 *
 * @param[in]  l_side               levá strana výrazu
 * @param[in]  r_side               pravá strana výrazu
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací FALSE
 */
int semantic_check_lside_rside(int l_side, int r_side);



/**
 * @brief      nastavuje return type aktuální funkce ve která je program zanořen
 *
 * @param      glob_var  ukazatel na globální proměnnou
 * @param[in]  local     pokud je pouze deklarace funkce je local = 0
 * @param[in]  ret_type  pokud je definice funkce je nastavena takto globální
 *                       proměnná
 * @param      name      klíč podlé kterého budeme vyhledávat v globální tabulce
 *                       symbolů a hledat zda nedošlo k redefinici, nedefinic
 *                       funkce
 * @param[in]  flag      příznak
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací FALSE
 */
int semantic_return_type(int* glob_var,int local,int ret_type, char* name, int flag);



/**
 * @brief      zjištuje zda je funkce definovaná
 *
 * @param      root  ukazatel na kořen tabulky symbolů
 * @param      name  klíč podle kterého se bude vyhledávat
 *
 * @return     0 = pokud funkce není definovaná v tabulce, 1 = pokud je funkce
 *             definovaná , -1 funkce není vůbec v tabulce
 */
int semantic_check_define(Ttnode_ptr* root, char* name);


/**
 * @brief      kotroluje zda funkce splnuje flag
 *
 * @param      root  ukazatel na tabulku symbolů
 * @param      t     ukazatel na aktuální token, ID, které se zpracovává
 */
void semantic_flag_use(Ttnode_ptr* root,TToken* t);



/**
 * @brief      kotroluje zda při definici není rozdílný počet parametrů jako při
 *             deklaraci
 *
 * @param[in]  root   ukazatel na tabulku symbolů
 * @param      name   klíč podle, kterého se bude vyhledávat
 * @param      param  ukazatel na načtené parametry
 *
 * @return     TRUE pokud semantická kontrola dopadla v pořádku, jinak vrací FALSE
 */
int semantic_check_params(Ttnode_ptr root,char* name, char* param);
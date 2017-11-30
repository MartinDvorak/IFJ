#include "semantic.h"
#include <string.h>

#define INT2DOUBLE			10
#define DOUBLE2INT			11

 
/**
 * @brief      Přidá prvek do pole operandů, pro generování kódu výrazů.
 *
 * @param      t              token, tj. prvek, který bude přidán
 * @param      operand_array  pole operandů pro generování
 * @param      ptr_to_array   ukazatel na první volnou pozici v poli
 * @param[in]  f_return_type  návratovná hodnota funkce, pro případ, že se jedná
 *                            o volání funkce ve výrazu
 */
void insert_operand_array(TToken* t, TExpr_operand* operand_array, int* ptr_to_array, int f_return_type);



/**
 * @brief      Uvolní pamět pole operandů a všech jeho prvků.
 *
 * @param      operand_array  pole operandů
 * @param[in]  size           počet využitých prvků
 */
void operand_array_destructor(TExpr_operand* operand_array, int size);



/**
 * @brief      Generuje kód výrazu, reprezentovaného postfixovým řetězcem.
 *             Bližší informace o operandech, resp. operátorech jsou uloženy v
 *             poli operandů, resp. operátorů (obs. i informace o implicitním
 *             přetypování). Výsledek výrazu je ponechán na vrcholu datového
 *             zásobníku.
 *
 * @param      operand_array  pole operandů
 * @param      postfix        postfixová reprezentace výrazu (řetězec)
 * @param      op_arr         pole operátorů
 */
void codegen_expression(TExpr_operand* operand_array, char* postfix, Toperation* op_arr);



/**
 * @brief      vypíše řetězec ".IFJcode17", tzv. začátek mezikódu
 */
void codegen_file_BEGIN();




/**
 * @brief      Generuje kód definice proměnné. Přiřazuje jí implicitní hodnotu
 *             dle datového typu.
 *
 * @param      name  jméno proměnné
 */
void codegen_dim(char* name);




/**
 * @brief      Generuje kód přiřazení. Uloží do proměnné hodnotu na vrcholu
 *             datového zásobníku.
 *
 * @param      name  jméno proměnné
 */
void codegen_assignment(char* name);




/**
 * @brief      Generuje kód čtení ze stdin. Nejprve vypíše řetězec !"? " na
 *             stdout, poté čeká na vstup.
 *
 * @param      t     token, reprezentující proměnnou, do které se bude načítat
 *                   hodnota
 */
void codegen_input(TToken* t);




/**
 * @brief      Generuje kód výpisu jedné hodnoty na stdout, reps. výrazu, jehož
 *             výsledek je na vrcholu datového zásobníku.
 */
void codegen_print();




/**
 * @brief      Generuje kód začátku scope sekce, tj. návěští a vytvoří lokální
 *             rámec.
 */
void codegen_scope();




/**
 * @brief      Generuje kód volání funkce, a uloží navrátovou hodnotu z vrcholu
 *             datového zásobníku do proměnné, pro další zpracování (ve výrazu).
 *
 * @param      name      jméno funkce
 * @param[in]  act_call  pom. proměnná pro gen. unikátního jména proměnné
 */
void codegen_func_call(char* name, int act_call);




/**
 * @brief      Uloží navrátovou hodnotu funkce z vrcholu datového zásobníku do
 *             proměnné, pro další zpracování (ve výrazu). Používá se u
 *             vestavěných funkcí.
 *
 * @param[in]  act_call  pom. proměnná pro gen. unikátního jména proměnné
 */
void codegen_get_func_result(int act_call);




/**
 * @brief      Generuje kód definice funkce, tj. návěští, a lokální rámec.
 *
 * @param      t     token, obsahující jméno proměnné
 */
void codegen_func_definition(TToken* t);




/**
 * @brief      Vloží na vrchol zásobníku implicitní návratovou hodnotu funkce,
 *             podle návratového typu. Pro případ, že se ve funkci neobjeví
 *             return.
 *
 * @param      t     token, obsahující návratový typ funkce
 */
void codegen_implicit_func_return(TToken* t);




/**
 * @brief      Zkopíruje skutečný parametr z datového zásobníku do formálního
 *             (proměnné funkce). Pozn. parametry jsou ze zásobníku kopírovány
 *             od posledního.
 *
 * @param      name  jméno proměnné (formálního parametru)
 */
void codegen_func_param(char* name);




/**
 * @brief      Generuje kód návratu z funkce. Přesune lokální rámec funkce, na
 *             dočasný. Kdekoliv v těle funkce, kde se objeví return.
 */
void codegen_func_return_inner();




/**
 * @brief      Generuje kód návratu z funkce. Přesune lokální rámec funkce, na
 *             dočasný. Na konci těla funkce.
 */
void codegen_func_return();




/**
 * @brief      Součást if-then-else, generuje skok na else větev v případě
 *             nesplnění podmínky.
 *
 * @param[in]  actual_if_id  id k identifikaci generovaného podmíněného výrazu
 */
void codegen_if_cond_jump(int actual_if_id);




/**
 * @brief      Součást if-then-else, generuje tvrdý skok na konec podm. výrazu
 *             po dokončení then větve a návěští pro else větev.
 *
 * @param[in]  actual_if_id  id k identifikaci generovaného podmíněného výrazu
 */
void codegen_else_label(int actual_if_id);




/**
 * @brief      Součást if-then-else, generuje návěští konce podmíněného výrazu.
 *
 * @param[in]  actual_if_id  id k identifikaci generovaného podmíněného výrazu
 */
void codegen_if_end_label(int actual_if_id);

/**WHILE LOOP****/




/**
 * @brief      Součást while-loop, generuje návěští vrcholu cyklu.
 *
 * @param[in]  actual_loop_id  id k identifikaci generovaného cyklu
 */
void codegen_loop_top_label(int actual_loop_id);




/**
 * @brief      Součást while-loop, generuje kód vyhodnocení podmínky cyklu a
 *             podmíněný skok z cyklu.
 *
 * @param[in]  actual_loop_id  id k identifikaci generovaného podmíněného výraz
 */
void codegen_loop_cond(int actual_loop_id);




/**
 * @brief      Součást while-loop, generuje tvrdý skok na vyhodnocení podmínky,
 *             tj. zacyklení, a návěští konce cyklu.
 *
 * @param[in]  actual_loop_id  id k identifikaci generovaného podmíněného výraz
 */
void codegen_loop_end(int actual_loop_id);




/**
 * @brief      Převede řetězec do formátu požadovaného jazykem IFJcode17, tj.
 *             neobsahuje žádné bílé znaky, povoluje některé escape sekvence.
 *
 * @param      source  vstupní řetězec
 *
 * @return     vrací řetězec převedený do požadovaného formátu
 */
char* string_convert_constant(char* source);




/**
 * @brief      Generuje kód funkce Length, tj. vrací délku řetězce daného
 *             parametrem. Výsledek funkce ponechává na vrcholu datového
 *             zásobníku. Návratový typ je integer.
 */
void codegen_buildin_length();




/**
 * @brief      Generuje kód funkce Asc, tj. vrací ASCII hodnotu znaku daného
 *             prvním parametrem, na pozici dané druhým parametrem. Výsledek
 *             ponechává na vrcholu datového zásobníku. Návratový typ je
 *             integer.
 */
void codegen_buildin_asc();




/**
 * @brief      Generuje kód funkce Chr, tj. vrací jednoznakový řetězec se znakem
 *             s ASCII hodnotou dané parametrem. Výsledek ponechává na vrcholu
 *             datového zásobníku.
 */
void codegen_buildin_chr();





/**
 * @brief      Generuje kód funkce SubStr, tj. vrací podřetězec v řetězci daném
 *             prvním parametrem, začínajícím znakem na pozici dané druhým
 *             parametrem (indexováno od 1) a o délce dané třetím parametrem.
 *             Vrací prázdný řetězec pokud je vstupní řetězec prázdný, nebo je
 *             index menší, nebo roven nule. Pokud je n menší, než nula, by
 *             podle parametrů podřetězec zasahoval za meze vstupního řetězce,
 *             vrací všechny znaky od indexu do konce řetězce. Výsledek
 *             ponechává na vrcholu datového zásobníku.
 */
void codegen_buildin_substr();
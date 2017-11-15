#include "semantic.h"

#define R_SIDE_FCALL 		0
#define R_SIDE_EXPR 		1
#define R_SIDE_BUILD_IN 	2
#define R_SIDE_NONE			3


//vypise zacatek vypisu mezikodu, tj. ".IFJcode17"
void codegen_file_BEGIN();

//prida prvek do pole operandu 
void insert_operand_array(TToken* t, TExpr_operand* operand_array, int* ptr_to_array);


//uvolni cele pole operandu 
void operand_array_destructor(TExpr_operand* operand_array, int size);

//generovani kodu vyrazu, nastavi cislo navratove hodnoty vyrazu
void codegen_expression(TExpr_operand* operand_array, char* postfix, Toperation* op_arr);


void codegen_dim(TToken* t, int r_side_type);

void codegen_assignment(TToken* t, int r_side_type);

//vytvori ramec LF
void codegen_scope();

//vytvori navesti, prevede TF na LF
void codegen_func_definition(TToken* t);

//zkopiruje skutecny parametr do formalniho na LF
void codegen_func_param(TToken* t, int param_no);

//prevede LF na TF a navrati se zpet z funkce 
void codegen_end_function();

//navratova hodnota funkce
void codegen_func_return();

//s konverzi retezce
void codegen_input(TToken* t);

//print jednoho vyrazu
void codegen_print();

/**IF-THEN-ELSE**/
//skok na else vetev, bool hodnota vyhodnoceni vyrazu je na vrcholu datoveho zasobniku, vycisti ho 
void codegen_if_cond_jump(int actual_if_id);

//vytvori skok na konec if-st za then vetvi a vytvori label pro skok na else vetev
void codegen_else_label(int actual_if_id);

//vygeneruje pouze label pro skok na konec if-statementu
void codegen_if_end_label(int actual_if_id);

/**WHILE LOOP****/
//nadefinuje promennou pro skok ven z cyklu a nevesti pro navrat na vyhodnoceni podminky
void codegen_loop_top_label(int actual_loop_id);

//vyhodnoceni podminky pro expressionu a pripadne skok z cyklu
void codegen_loop_cond(int actual_loop_id);

//skok nad cyklus, a generovani labelu konec cyklu
void codegen_loop_end(int actual_loop_id);

/****************/

//pouze skoci na kod funkce
void codegen_func_call(TToken* t);

//vytvori prazdny ramec pro fci
void codegen_empty_func_frame();

//prida parametr na LF s poradovym cislem 
void codegen_func_call_give_param(TToken* t, int param_no);

//prevede retezec do formatu pro IFJcode17 (zadne bile znaky -> esc sekvence)
char* string_convert_constant(char* source);

//prevede promennou retezec do formatu IFJcode17, v mezikodu
void string_convert_input(TToken* t);


/**BUILD IN FUNKCE***********/

void codegen_buildin_length(TToken* t);

void codegen_buildin_asc(TToken* string_token, TToken* position_token);

void codegen_buildin_chr(TToken* t);

void codegen_buildin_substr(TToken* string_token, TToken* beg_token, TToken* len_token);
#include "semantic.h"

//prida prvek do pole operandu 
void insert_operand_array(TToken* t, TExpr_operand* operand_array, int* ptr_to_array);


//uvolni cele pole operandu 
void operand_array_destructor(TExpr_operand* operand_array, int size);

//generovani kodu vyrazu, nastavi cislo navratove hodnoty vyrazu
void codegen_expression(TExpr_operand* operand_array, char* postfix, Toperation* op_arr, int* expr_ret_id);


void codegen_dim(TToken* t);


void codegen_scope();


void codegen_input(TToken* t);


void codegen_print(int expr_ret_id);


//prevede retezec do formatu pro IFJcode17 (zadne bile znaky -> esc sekvence)
char* string_convert_constant(char* source);

//prevede promennou retezec do formatu IFJcode17, v mezikodu
void string_convert_input(TToken* t);

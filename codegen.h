#include "semantic.h"

//prida prvek do pole operandu 
void insert_operand_array(TToken* t, TExpr_operand* operand_array, int* ptr_to_array);


//uvolni cele pole operandu 
void operand_array_destructor(TExpr_operand* operand_array, int size);


void codegen_expression(TExpr_operand* operand_array, char* postfix, Toperation* op_arr);


void codegen_dim(TToken* t);

//prevede retezec do formatu pro IFJcode17 (zadne bile znaky -> esc sekvence)
char* string_convert(char* source);




/*

int generate_if_then_else();

int generate_while_loop();

int generate_input();

void generate_Dim();

int generate_Dim_expr();

int generate_func_declaration();

int generate_func_definition();

int generate_func_call();

int generate_print();

int generate_assignment();

*/


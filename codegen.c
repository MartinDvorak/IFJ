#include "codegen.h"


void codegen_file_BEGIN(){

	printf(".IFJcode17\n");
}

/****ZPRACOVANI VYRAZU****************************************************************/

//prida prvek do pole operandu 
void insert_operand_array(TToken* t, TExpr_operand* operand_array, int* ptr_to_array){

	if(t->type == INT_V){
		//int konstanta
		operand_array[*ptr_to_array].semantic_type = INTEGER;
		operand_array[*ptr_to_array].type = INT_V;
		operand_array[*ptr_to_array].int_v = t->int_v;
		operand_array[*ptr_to_array].name = NULL;
		operand_array[*ptr_to_array].string = NULL;
	}
	else if(t->type == FLOAT_V){
		//float konstanta
		operand_array[*ptr_to_array].semantic_type = DOUBLE;
		operand_array[*ptr_to_array].type = FLOAT_V;
		operand_array[*ptr_to_array].float_v = t->float_v;
		operand_array[*ptr_to_array].name = NULL;
		operand_array[*ptr_to_array].string = NULL;
	}
	else if(t->type == STRING_V){
		//string konstanta
		operand_array[*ptr_to_array].semantic_type = STRING;
		operand_array[*ptr_to_array].type = STRING_V;
		if((operand_array[*ptr_to_array].string =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
		}
		strcpy(operand_array[*ptr_to_array].string, t->string);
		operand_array[*ptr_to_array].name = NULL;
	}
	else if(t->type == ID){
		//promenna
		Tdata symb;
		Tdata* symbTmp = &symb;
		search_tree(root_local,t->string,symbTmp);
		if(symbTmp->type == INTEGER){
			//promenna integer
			operand_array[*ptr_to_array].semantic_type = INTEGER;
			operand_array[*ptr_to_array].type = INTEGER;
			if((operand_array[*ptr_to_array].name =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
			}
			strcpy(operand_array[*ptr_to_array].name,t->string);
			operand_array[*ptr_to_array].string = NULL;
		}
		else if(symbTmp->type == DOUBLE){
			//promenna float
			operand_array[*ptr_to_array].semantic_type = DOUBLE;
			operand_array[*ptr_to_array].type = DOUBLE;
			if((operand_array[*ptr_to_array].name =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
			}
			strcpy(operand_array[*ptr_to_array].name,t->string);
			operand_array[*ptr_to_array].string = NULL;
		}
		else if(symbTmp->type == STRING){
			//promenna string
			operand_array[*ptr_to_array].semantic_type = STRING;
			operand_array[*ptr_to_array].type = STRING;
			if((operand_array[*ptr_to_array].name =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
			}
			strcpy(operand_array[*ptr_to_array].name,t->string);
			operand_array[*ptr_to_array].string = NULL;
		}
	}

	//pridani do pole operandu
	*ptr_to_array += 1;
}


//uvolni cele pole operandu 
void operand_array_destructor(TExpr_operand* operand_array, int size){

	for(int i = 0; i < size; i++){
		//pruchod pres prvky
		if(operand_array[i].type == STRING_V){
			//konstanta string
			free(operand_array[i].string);
		}
		if(operand_array[i].type == ID){
			//promenna, uvolnit jmeno
			free(operand_array[i].name);
		}
	}

	//uvolneni celeho pole
	free(operand_array);
}


//generovani kodu pro vyraz
void codegen_expression(TExpr_operand* operand_array, char* postfix, Toperation* op_arr){
	
	int operand_index = 0;	//kolik operandu v poli se uz proslo
	int operator_index = 0;	//kolik operatoru v poli se uz proslo
	
	for(int i = 0; postfix[i] != '$'; i++){

		if(postfix[i] == 'i'){
			//promenna nebo konstanta
			if((operand_array[operand_index].type == INTEGER) || (operand_array[operand_index].type == DOUBLE)
				|| (operand_array[operand_index].type == STRING)){
				//promenna
				printf("PUSHS LF@%s\n", operand_array[operand_index].name);

			}
			else if((operand_array[operand_index].type == INT_V) || (operand_array[operand_index].type == FLOAT_V)
				|| (operand_array[operand_index].type == STRING_V)){
				//konstanta
				char* str_out = NULL;
				switch(operand_array[operand_index].type){
					case INT_V:
						printf("PUSHS int@%d\n", operand_array[operand_index].int_v);	
						break;
					case FLOAT_V:
						printf("PUSHS float@%g\n", operand_array[operand_index].float_v);
						break;
					case STRING_V:
						str_out = string_convert_constant(operand_array[operand_index].string);				
						printf("PUSHS string@%s\n", str_out);
						free(str_out);
						break;
				} 
			}

			operand_index++;	
		}
		else{
			//operator
			Toperation act = op_arr[operator_index];

			if(act.l_convert){
				//levy operand konverze
				printf("CREATEFRAME\n");
				printf("DEFVAR TF@$tmp1\n");
				printf("DEFVAR TF@$tmp2\n");

				printf("POPS TF@$tmp1\n");
				printf("POPS TF@$tmp2\n");
				printf("INT2FLOATS\n");
				printf("PUSHS TF@$tmp2\n");
				printf("PUSHS TF@$tmp1\n");
			}
			if(act.r_convert){
				//pravy operand konverze
				printf("CREATEFRAME\n");
				printf("DEFVAR TF@$tmp1\n");

				printf("POPS TF@$tmp1\n");
				printf("INT2FLOATS\n");
				printf("PUSHS TF@$tmp1\n");
			}

			switch(act.op){
				//provedeni operace
				case '+':
					if((operand_array[operand_index-1].type == STRING) || 
						(operand_array[operand_index-1].type == STRING_V)){
						//jde o retezce
						printf("CREATEFRAME\n");
						printf("DEFVAR TF@$tmp_op\n");
						printf("DEFVAR TF@$tmp1\n");
						printf("DEFVAR TF@$tmp2\n");
						printf("DEFVAR TF@$tmp_res\n");

						printf("POPS TF@$tmp_op\n");
						printf("POPS TF@$tmp1\n");
						printf("POPS TF@$tmp2\n");
						printf("CONCAT TF@$tmp_res TF@$tmp2 TF@$tmp1\n");
						printf("PUSHS TF@$tmp_res\n");
					} 
					else{
						//integery nebo floaty
					printf("ADDS\n");
					}
					break;
				case '-':
					printf("SUBS\n");
					break;
				case '*':
					printf("MULS\n");
					break;
				case '/':
					printf("DIVS\n");
					break;
				case 'M':
					//prevede oba na float, podeli, osekne na int
					printf("CREATEFRAME\n");
					printf("DEFVAR TF@tmp_op\n");
					printf("DEFVAR TF@tmp1\n");

					printf("POPS TF@tmp_op\n");
					printf("INT2FLOATS\n");
					printf("POPS TF@tmp1\n");
					printf("INT2FLOATS\n");
					printf("PUSHS TF@tmp1\n");
					printf("DIVS\n");
					printf("FLOAT2INTS\n");
					break;
				case 'N':
					printf("EQS\n");
					printf("NOTS\n");
					break;
				case '=':
					printf("EQS\n");
					break;
				case 'L':
					printf("LTS\n");
					break;
				case 'G':
					printf("GTS\n");
					break;
				case 'S':
					printf("GTS\n");
					printf("NOTS\n");
					break;
				case 'R':
					printf("LTS\n");
					printf("NOTS\n");
					break;
			}
			operator_index++;
		}

		if(postfix[i+1] == '\0'){
			

			/**********************************************
			**vysledek vyrazu zustane na zasobniku, 
			**je potreba ho nasledne vycistit, aby se na nem nehromadily vysledky
			*/
		}
	}
}


/****KONEC ZPRACOVANI VYRAZU**********************************************************/



//zatim jen prototyp
void codegen_dim(char* name){

	Tdata data;
	Tdata* tdata = &data;

	search_tree(root_local, name, tdata);

	printf("DEFVAR LF@%s\n", name);

	int type = tdata->type;

	switch(type){

				case INTEGER:
					printf("MOVE LF@%s int@0\n", name);
					break;
				case DOUBLE: 
					printf("MOVE LF@%s float@0.0\n", name);
					break;
				case STRING:
					printf("MOVE LF@%s string@!\"\"\n", name);
					break;
			}
}

void codegen_dim_r_side(char* name, int r_side_type){

	switch(r_side_type){

		case R_SIDE_NONE:
			break;
		case R_SIDE_EXPR:
			//expression
			printf("POPS LF@%s\n", name);
			printf("CLEARS\n");
			break;
		case R_SIDE_BUILD_IN:
			//build in function call
			printf("MOVE LF@%s TF@&retval_function\n", name);
			break;
		case R_SIDE_FCALL:
			//function call
			printf("MOVE LF@%s TF@&retval_function\n", name);
			break;
	}
}

void codegen_assignment(char* name, int r_side_type){

	switch(r_side_type){
		case R_SIDE_EXPR:
			//expression
			printf("POPS LF@%s\n", name);
			printf("CLEARS\n");
			break;
		case R_SIDE_BUILD_IN:
			//build in function call
			printf("MOVE LF@%s TF@&retval_function\n", name);
			break;
		case R_SIDE_FCALL:
			//function call
			printf("MOVE LF@%s TF@&retval_function\n", name);
			break;
	}
}

void codegen_input(TToken* t){

	Tdata sdata;
	Tdata* data = &sdata;
	search_tree(root_local,t->string,data);

	switch(data->type){

		case DOUBLE:
			printf("READ LF@%s double\n", t->string);
			break;
		case INTEGER:
			printf("READ LF@%s int\n", t->string);
			break;
		case STRING:
			printf("READ LF@%s string\n", t->string);
			//prevod retezce na format IFJcode17 (bez bilych znaku)
			string_convert_input(t);
			break;
	}
}

//print jednoho vyrazu
void codegen_print(){

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@$retval_expr\n");
	printf("POPS TF@$retval_expr\n");
	printf("WRITE TF@retval_expr\n");
	printf("CLEARS\n");
}

//vytvori LF
void codegen_scope(){

	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

}


/****FUNKCE, DEFINICE A VOLANI**********************************************************************/

//vytvori navesti, prevede TF na LF
void codegen_func_definition(TToken* t){

	printf("LABEL @func@%s\n", t->string);
	printf("PUSHFRAME\n");
}

//zkopiruje skutecny parametr do formalniho na LF
void codegen_func_param(TToken* t, int param_no){

	printf("DEFVAR LF@%s\n", t->string);
	printf("MOVE LF@%s LF@$f_param%d\n", t->string, param_no);

}

//prevede LF na TF a navrati se zpet z funkce (RETURN), , navratova hodnota predat ?
void codegen_end_function(){

	printf("POPFRAME\n");
	printf("RETURN\n");
}

void codegen_func_return(){

	printf("DEFVAR LF@&retval_function\n");
	printf("POPS LF@&retval_function\n");
	printf("CLEARS\n");
}

//pouze skoci na kod funkce
void codegen_func_call(char* f_name){

	printf("CALL @func@%s\n", f_name);

}

//vytvori prazdny ramek, ktery funkce pushne na LF top
void codegen_empty_func_frame(){

	printf("CREATEFRAME\n");
}

void codegen_func_call_give_param(TToken* t, int param_no){

	switch(t->type){

		case ID:
			printf("DEFVAR TF@$f_param%d\n", param_no);
			printf("MOVE TF@$f_param%d LF@%s\n", param_no, t->string);
			break;
		case FLOAT_V:
			printf("DEFVAR TF@$f_param%d\n", param_no);
			printf("MOVE TF@$f_param%d float@%g\n", param_no, t->float_v);
			break;
		case INT_V:
			printf("DEFVAR TF@$f_param%d\n", param_no);
			printf("MOVE TF@$f_param%d int@%d\n", param_no, t->int_v);
			break;
		case STRING_V:
			printf("DEFVAR TF@$f_param%d\n", param_no);
			char* str = string_convert_constant(t->string);
			printf("MOVE TF@$f_param%d string@%s\n", param_no, str);
			break;
	}
}


/****IF THEN ELSE STATEMENT************************************************************************/

//skok na else vetev, bool hodnota vyhodnoceni vyrazu je na vrcholu datoveho zasobniku, vycisti ho 
void codegen_if_cond_jump(int actual_if_id){

	printf("DEFVAR LF$if_cond_state_%d\n", actual_if_id);
	printf("POPS LF$if_cond_state_%d\n", actual_if_id);
	printf("JUMPIFNEQ $else_branch_%d LF$if_cond_state_%d bool@true\n", actual_if_id, actual_if_id);
	printf("CLEARS\n");
}

//vytvori skok na konec if-st za then vetvi a vytvori label pro skok na else vetev
void codegen_else_label(int actual_if_id){

	printf("JUMP $end_if_%d\n", actual_if_id);
	printf("LABEL $else_branch_%d\n", actual_if_id);

}

//vygeneruje pouze label pro skok na konec if-statementu
void codegen_if_end_label(int actual_if_id){

	printf("LABEL $end_if_%d\n", actual_if_id);
}


/****WHILE LOOP***********************************************************************************/

//nadefinuje promennou pro skok ven z cyklu a nevesti pro navrat na vyhodnoceni podminky
void codegen_loop_top_label(int actual_loop_id){

	printf("DEFVAR LF@loop_state_%d\n", actual_loop_id);
	printf("LABEL $loop_top_%d\n", actual_loop_id);
}

void codegen_loop_cond(int actual_loop_id){

	//ziskani vyhodnoceni podminky ze zasobniku
	printf("POPS LF@loop_state_%d\n", actual_loop_id);
	printf("CLEARS\n");	

	printf("JUMPIFNEQ $loop_end_%d LF@loop_state_%d bool@true\n", actual_loop_id, actual_loop_id);
}

void codegen_loop_end(int actual_loop_id){

	printf("JUMP $loop_top_%d\n", actual_loop_id);
	printf("LABEL $loop_end_%d\n", actual_loop_id);
}

void codegen_buildin_length(TToken* t){

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@&retval_function\n");	//chova se jako by byla funkci

	if(t->type == ID){

		printf("STRLEN TF@&retval_function LF@%s\n", t->string);
	}
	else{
		//type == STRING_V
		char* str = string_convert_constant(t->string);
		printf("STRLEN TF@&retval_function string@%s\n", str);
	}
}


/****BUILD IN FUNKCE*********************************************************************************/

void codegen_buildin_asc(TToken* string_token, TToken* position_token){

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@&retval_function\n");	//chova se jako by byla funkci

	printf("DEFVAR TF@$position\n");

	if(position_token->type == ID){
		printf("MOVE TF@$position LF@%s\n", position_token->string);
		printf("ADD TF@$position TF@$position int@1\n");	//indexovani od 1, ne od nuly
	}
	else{
		//INT_V
		printf("MOVE TF@$position int@%d\n", position_token->int_v);
		printf("ADD TF@$position TF@$position int@1\n");	//indexovani od 1, ne od nuly
	}

	if(string_token->type == ID){

		printf("STRI2INT TF@&retval_function LF@%s TF@$position\n", string_token->string);
	}
	else{
		//type = STRING_V
		char* str = string_convert_constant(string_token->string);
		printf("STRI2INT TF@&retval_function string@%s TF@$position\n", str);
	}
}

void codegen_buildin_chr(TToken* t){

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@&retval_function\n");	//chova se jako by byla funkci

	if(t->type == ID){

		printf("INT2CHAR TF@&retval_function LF@%s\n", t->string);
	}
	else{
		//type == INT_V
		printf("INT2CHAR TF@&retval_function int@%d\n", t->int_v);	
	}
}

void codegen_buildin_substr(TToken* string_token, TToken* beg_token, TToken* len_token){

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@&retval_function\n");	//chova se jako by byla funkci

	printf("DEFVAR TF@*input\n");
	if(string_token->type == ID){
		printf("MOVE TF@*input LF@%s\n", string_token->string);
	}
	else{
		//STRING_V
		char* str = string_convert_constant(string_token->string);
		printf("MOVE TF@*input string@%s\n", str);
	}

	printf("DEFVAR TF@*index\n");
	if(beg_token->type == ID){
		printf("MOVE TF@*index LF@%s\n", beg_token->string);
	}
	else{
		//type == INT_V
		printf("MOVE TF@*index int@%d\n", beg_token->int_v);
	}
	printf("ADD TF@*index TF@*index int@1\n");	//posun o 1, index od 1 ne od nuly

	printf("DEFVAR TF@*out_len\n");
	if(len_token->type == ID){
		printf("MOVE TF@*out_len LF@%s\n", len_token->string);
	}
	else{
		//type == INT_V
		printf("MOVE TF@*out_len int@%d\n", len_token->int_v);
	}

	printf("DEFVAR TF@*str_len\n");
	printf("STRLEN TF@*str_len TF@*input\n");	//delka vstupniho retezce

	printf("PUSHS TF@*out_len\n");
	printf("PUSHS int@0\n");
	printf("LTS\n");
	printf("PUSHSTF@*out_len\n");
	printf("PUSHS TF@*str_len\n");
	printf("GTS\n");
	printf("ORS\n");							// (out_len < 0) ||	(out_len > Length(in_string))

	printf("DEFVAR TF@*state\n");				//vysledek porovnani
	printf("POPS TF@*state\n");

	printf("DEFVAR TF@*end_index\n");
	printf("JUMPIFEQ $*else_branch TF@*state bool@true\n");		// if(out_len < 0) ||	(out_len > Length(in_string))

		printf("MOVE TF@*end_index TF@*str_len\n");				//end_index = length(in_string)

	printf("JUMP $*end_if\n");
	printf("LABEL $*else_branch\n");			//else

		printf("ADD TF@*end_index TF@*index TF@*out_len\n");	//end_index = index = out_len

	printf("LABEL $*end_if\n");					//end if 

	printf("DEFVAR TF@*state_loop\n");
	printf("DEFVAR TF@*tmp_char\n");

	/**WHILE LOOP**/
	printf("LABEL $*loop_top\n");
	printf("GT TF@*state_loop TF@*index TF@*end_index\n");
	printf("JUMPIFEQ $*end_loop TF@*state_loop bool@true\n");	//while (index <= end_index)

		printf("GETCHAR TF@*tmp_char TF@*input TF@*index\n");
		printf("CONCAT TF@&retval_function TF@&retval_function TF@*tmp_char\n");	//ret = ret = char[index]

	printf("ADD TF@*index TF@*index int@1\n");	//i++
	printf("JUMP $*loop_top\n");
	printf("LABEL $*end_loop\n");	//end loop
}

/****PREVOD RETEZCU********************************************************************************************/

//prevede retezec na vystup. format
char* string_convert_constant(char* source){

	int src_len = strlen(source);
	int out_len;

	for(out_len = 0; out_len < src_len; out_len++){
		if((source[out_len] >= 0 && source[out_len] <= 32) || (source[out_len] == 92))
			out_len+= 3;
	}
	
	char* output;
	output = malloc(sizeof(char)*(out_len+1));

	for(int i = 0, j = 0; i < src_len; i++, j++){
		if((source[i] >= 0 && source[i] <= 32) || (source[i] == 92)){
			output[j] = '\\';
			output[j+1] = '0';

			char* tmp;
			if((tmp = malloc(sizeof(char)*3)) == NULL)
				exit(99);
			sprintf(tmp, "%02d", source[i]);
			output[j+2] = tmp[0];
			output[j+3] = tmp[1];
			free(tmp);
			j+=3;
		}
		else{
			output[j] = source[i];
		}		
	}
	return output;
}

//prevede promennou retezec do formatu IFJcode17, v mezikodu
void string_convert_input(TToken* t){

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@$input\n");
	printf("MOVE TF@$input LF@%s\n", t->string);	//vstupni retezec do tmp promenne
	printf("DEFVAR TF@$output\n");		//promenna pro vystupni retezec

	printf("DEFVAR TF@$input_len\n");
	printf("STRLEN TF@$input_len TF@$input\n");	//zjisteni delky vstupniho retezce

	printf("DEFVAR TF@$i\n");	//pocitadlo cyklu
	printf("MOVE TF@$i int@0\n");

	/*WHILE LOOP***********************************************************/
	printf("LABEL $loop\n");
	printf("JUMPIFEQ $end_loop TF@$input_len int@0 \n"); //while STRLEN != 0

		printf("DEFVAR TF@$char_no\n");
		printf("STRI2INT TF@$char_no TF@$input \n");	//zjisteni ASCII znaku


		/*IF char = \000 - /032******************************/
		printf("DEFVAR TF@$state\n");	//bool state
		printf("GT TF@$state TF@$char_no int@32\n");
		printf("JUMPIFEQ $GT32 TF@$state bool@true\n");			//if char < 32

			//char je mezi 00 a 32
			/*IF STATEMENT, PODLE VELIKOSTI, PO DESETI*/
			printf("DEFVAR TF@$char_state\n");
			printf("GT TF@$char_state TF@$char_no int@9\n");
			printf("JUMPIFEQ $g9 TF@$char_state bool@true\n");

				//0-9
				printf("DEFVAR TF@$converted_char_no\n");
				printf("ADD TF@$char_no TF@$char_no int@48\n");	//posunu se z cisla na znak
				printf("INT2CHAR TF@$converted_char_no TF@$char_no\n"); // prevedu na znak

				printf("CONCAT TF@$output TF@$output string@\\00 \n");
				printf("CONCAT TF@$output TF@$output TF@$converted_char_no\n");	//pripojim na vystupni string

			printf("JUMP $endif_chs\n");
			printf("LABEL $g9\n");
			printf("GT TF@$char_state TF@$char_no int@19\n");
			printf("JUMPIFEQ $g19 TF@$char_state bool@true\n");

				//10-19
				printf("DEFVAR TF@$converted_char_no\n");
				printf("SUB TF@$char_no TF@$char_no int@10\n"); //dostanu jednotky
				printf("ADD TF@$char_no TF@$char_no int@48\n");	//posunu se z cisla na znak, jednotky
				printf("INT2CHAR TF@$converted_char_no TF@$char_no\n"); // prevedu na znak, jednotky

				printf("CONCAT TF@$output TF@$output string@\\01 \n");	//uz pripojuji 1, jako "+10"
				printf("CONCAT TF@$output TF@$output TF@$converted_char_no\n");	//pripojim na vystupni string

			printf("JUMP $endif_chs\n");
			printf("LABEL $g19\n");
			printf("GT TF@$char_state TF@$char_no int@29\n");
			printf("JUMPIFEQ $g29 TF@$char_state bool@true\n");

				//20-29
				printf("DEFVAR TF@$converted_char_no\n");
				printf("SUB TF@$char_no TF@$char_no int@20\n"); //dostanu jednotky
				printf("ADD TF@$char_no TF@$char_no int@48\n");	//posunu se z cisla na znak, jednotky
				printf("INT2CHAR TF@$converted_char_no TF@$char_no\n"); // prevedu na znak, jednotky

				printf("CONCAT TF@$output TF@$output string@\\02 \n");	//uz pripojuji 2, jako "+20"
				printf("CONCAT TF@$output TF@$output TF@$converted_char_no\n");	//pripojim na vystupni string

			printf("JUMP $endif_chs\n");
			printf("LABEL $g29\n");

				//30-32
				printf("DEFVAR TF@$converted_char_no\n");
				printf("SUB TF@$char_no TF@$char_no int@30\n"); //dostanu jednotky
				printf("ADD TF@$char_no TF@$char_no int@48\n");	//posunu se z cisla na znak, jednotky
				printf("INT2CHAR TF@$converted_char_no TF@$char_no\n"); // prevedu na znak, jednotky

				printf("CONCAT TF@$output TF@$output string@\\03 \n");	//uz pripojuji 3, jako "+30"
				printf("CONCAT TF@$output TF@$output TF@$converted_char_no\n");	//pripojim na vystupni string

			printf("LABEL $endif_chs\n");


		printf("JUMP $endif\n");
		printf("LABEL $GT32\n");	//vetsi nez \032
		printf("JUMPIFNEQ $normal_char TF@$char_no int@92\n");	//else if char == 92

			//char == 92, pouze pripoji retezec \092
			printf("CONCAT TF@$output TF@$output string@\\092\n");

		printf("LABEL $endif\n");
		printf("LABEL $normal_char\n");							//else

			//normalni char, pouze kopiruje vstup na vystup
			printf("DEFVAR TF@$char\n");
			printf("GETCHAR TF@$char TF@$input TF@$i\n");
			printf("CONCAT TF@$output TF@$output TF@$char\n");

		printf("LABEL $endif\n");	//konec if
		/*END IF********************************************/



		printf("ADD TF@$i TF@$i int@1\n");	//i++
	printf("JUMP $loop\n");
	printf("LABEL $end_loop\n");	//end loop
	/*END WHILE LOOP*******************************************************/

	//zapsani formatovaneho retezce
	printf("MOVE LF@%s\n TF@$output", t->string);
}
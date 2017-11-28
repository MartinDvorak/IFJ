#include "codegen.h"


void codegen_file_BEGIN(){

	printf(".IFJcode17\n");
}

/****ZPRACOVANI VYRAZU****************************************************************/

//prida prvek do pole operandu 
void insert_operand_array(TToken* t, TExpr_operand* operand_array, int* ptr_to_array, int f_return_type){

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
		//printf("!!!!!!!!!!!!!!!!!!!!! %g\n", t->`_v);
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
		int state;
		if((state = search_tree(root_local,t->string,symbTmp)) == FALSE){
			//interni promenna navratu funkce
			operand_array[*ptr_to_array].semantic_type = f_return_type;
			operand_array[*ptr_to_array].type = f_return_type;
			if((operand_array[*ptr_to_array].name =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
			}
			strcpy(operand_array[*ptr_to_array].name,t->string);
			operand_array[*ptr_to_array].string = NULL;
		}

		else if(symbTmp->type == INTEGER){
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

	for(int i = 0; i < (size-1); i++){
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
	static int call_counter = 0;
	int act_call = call_counter;
	call_counter++;
	
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
				printf(	"DEFVAR LF@$tmp1l_%d\n"
						
						"POPS LF@$tmp1l_%d\n"
						"INT2FLOATS\n"
						"PUSHS LF@$tmp1l_%d\n"
						,act_call, act_call, act_call);
			}
			if(act.r_convert){
				//pravy operand konverze
				printf("INT2FLOATS\n");
			}

			switch(act.op){
				//provedeni operace
				case '+':
					if((operand_array[operand_index-1].type == STRING) || 
						(operand_array[operand_index-1].type == STRING_V)){
						//jde o retezce
						printf(	"DEFVAR LF@$tmp1_conc_%d\n"
								"DEFVAR LF@$tmp2_conc_%d\n"
								"DEFVAR LF@$tmp_res\n"
								
								"POPS LF@$tmp1_conc_%d\n"
								"POPS LF@$tmp2_conc_%d\n"
								"CONCAT LF@$tmp_res_conc_%d LF@$tmp2_conc_%d LF@$tmp1_conc_%d\n"
								"PUSHS LF@$tmp_res_conc_%d\n"
								,act_call, act_call, act_call, act_call, act_call, act_call, act_call, act_call);
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
					printf(	"DEFVAR LF@$tmp1_mul_%d\n"
							
							"INT2FLOATS\n"
							"POPS LF@$tmp1_mul_%d\n"
							"INT2FLOATS\n"
							"PUSHS LF@$tmp1_mul_%d\n"
							"DIVS\n"
							"FLOAT2INTS\n"
							,act_call, act_call, act_call);
					break;
				case 'N':
					printf(	"EQS\n"
							"NOTS\n");
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
					printf(	"GTS\n"
							"NOTS\n");
					break;
				case 'R':
					printf(	"LTS\n"
							"NOTS\n");
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
	static int call_counter = 0;


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
					printf("MOVE LF@%s string@\n", name);
					break;
			}

	call_counter++;
}

void codegen_assignment(char* name){

			//expression
			printf("POPS LF@%s\n", name);
			printf("CLEARS\n");
}

void codegen_input(TToken* t){

	Tdata sdata;
	Tdata* data = &sdata;
	search_tree(root_local,t->string,data);

	printf("WRITE string@?\\032\n");

	switch(data->type){

		case DOUBLE:
			printf("READ LF@%s float\n", t->string);
			break;
		case INTEGER:
			printf("READ LF@%s int\n", t->string);
			break;
		case STRING:
			printf("READ LF@%s string\n", t->string);
			break;
	}
}

//print jednoho vyrazu
void codegen_print(){

	static int call_counter = 0;
	call_counter++;

	printf(	"DEFVAR LF@$retval_expr_%d\n"
			"POPS LF@$retval_expr_%d\n"
			"WRITE LF@$retval_expr_%d\n"
			"CLEARS\n"
			,call_counter, call_counter, call_counter);
}

//vytvori LF
void codegen_scope(){

	printf(	"LABEL &&&scope\n"
			"CREATEFRAME\n"
			"PUSHFRAME\n");

}


/****FUNKCE, DEFINICE A VOLANI**********************************************************************/

//vytvori navesti, prevede TF na LF
void codegen_func_definition(TToken* t){

	printf(	"JUMP &&&scope\n"
			"LABEL &func&%s\n"
			"PUSHFRAME\n"
			"DEFVAR LF@&retval_function\n", t->string);
}

//vlozi do navratove hodnoty implicitni podle nav. typu, pro pripad, ze se ve funkci nenachazi return
void codegen_implicit_func_return(TToken* t){

	if(t->type == INTEGER){
		printf("MOVE LF@&retval_function int@0\n");
	}
	else if(t->type == DOUBLE){
		printf("MOVE LF@&retval_function float@0.0\n");
	}
	else{
		//t->type == STRING
		printf("MOVE LF@&retval_function string@\n");

	}

}

//zkopiruje skutecny parametr do formalniho na LF
void codegen_func_param(TToken* t, int param_no){

	printf(	"DEFVAR LF@%s\n"
			"MOVE LF@%s LF@$f_param%d\n", t->string, t->string, param_no);

}

//prevede LF na TF a navrati se zpet z funkce (RETURN), , navratova hodnota predat ?
void codegen_end_function(){

	printf(	"POPFRAME\n"
			"RETURN\n");
}

void codegen_func_return(){

	printf(	"POPS LF@&retval_function\n"
			"POPFRAME\n"
			"RETURN\n");
}

/*
//pouze skoci na kod funkce
void codegen_func_call(char* f_name){

	printf("CALL &func&%s\n", f_name);

}
*/


//vytvori prazdny ramek, ktery funkce pushne na LF top
void codegen_empty_func_frame(){

	printf("CREATEFRAME\n");
}

void codegen_func_call_give_param(int param_no){


			printf("DEFVAR TF@$f_param%d\n", param_no);
			printf("POPS TF@$f_param%d\n", param_no);
			//if(covert_type == INT2DOUBLE) printf("INT2FLOAT TF@$f_param%d TF@$f_param%d\n", param_no, param_no);
			//if(covert_type == DOUBLE2INT) printf("FLOAT2R2OINT TF@$f_param%d TF@$f_param%d\n", param_no, param_no);
}


/****IF THEN ELSE STATEMENT************************************************************************/

//skok na else vetev, bool hodnota vyhodnoceni vyrazu je na vrcholu datoveho zasobniku, vycisti ho 
void codegen_if_cond_jump(int actual_if_id){

	printf(	"PUSHS bool@true\n"
			"JUMPIFNEQS $else_branch_%d\n"
			, actual_if_id);
			//bylo tu CLEARS
}

//vytvori skok na konec if-st za then vetvi a vytvori label pro skok na else vetev
void codegen_else_label(int actual_if_id){

	printf(	"JUMP $end_if_%d\n"
			"LABEL $else_branch_%d\n", actual_if_id, actual_if_id);

}

//vygeneruje pouze label pro skok na konec if-statementu
void codegen_if_end_label(int actual_if_id){

	printf("LABEL $end_if_%d\n", actual_if_id);
}


/****WHILE LOOP***********************************************************************************/

//nadefinuje promennou pro skok ven z cyklu a nevesti pro navrat na vyhodnoceni podminky
void codegen_loop_top_label(int actual_loop_id){

	printf("LABEL $loop_top_%d\n", actual_loop_id);
}

void codegen_loop_cond(int actual_loop_id){

	printf(	"PUSHS bool@true\n"
			"JUMPIFNEQS $loop_end_%d\n"
			, actual_loop_id);	
			//bylo CLEARS
}

void codegen_loop_end(int actual_loop_id){

	printf(	"JUMP $loop_top_%d\n"
			"LABEL $loop_end_%d\n", actual_loop_id, actual_loop_id);
}


/****BUILD IN FUNKCE*********************************************************************************/

void codegen_buildin_length(TToken* t){

	printf(	"CREATEFRAME\n"
			"DEFVAR TF@&retval_function\n");	//chova se jako by byla funkci

	if(t->type == ID){

		printf("STRLEN TF@&retval_function LF@%s\n", t->string);
	}
	else{
		//type == STRING_V
		char* str = string_convert_constant(t->string);
		printf("STRLEN TF@&retval_function string@%s\n", str);
	}
}


void codegen_buildin_asc(TToken* string_token, TToken* position_token, int convert_param){

	static int call_counter = 0;

	printf(	"CREATEFRAME\n"
			"DEFVAR TF@&retval_function\n"			//chova se jako by byla funkci
			"MOVE TF@&retval_function int@0\n"
			
			"DEFVAR TF@$position\n");

	if(position_token->type == ID){
		printf("MOVE TF@$position LF@%s\n", position_token->string);
		if(convert_param == DOUBLE2INT) printf("FLOAT2R2OINT TF@$position TF@$position\n");
		printf("SUB TF@$position TF@$position int@1\n");	//indexovani od 1, ne od nuly
	}
	else if(position_token->type == INT_V){
		printf("MOVE TF@$position int@%d\n", position_token->int_v);
		if(convert_param == DOUBLE2INT) printf("FLOAT2R2OINT TF@$position TF@$position\n");
		printf("SUB TF@$position TF@$position int@1\n");	//indexovani od 1, ne od nuly
	}
	else{
		//FLOAT_V
		printf(	"MOVE TF@$position float@%g\n"
				"FLOAT2R2OINT TF@$position TF@$position\n"
				"SUB TF@$position TF@$position int@1\n", position_token->float_v);	//indexovani od 1, ne od nuly
	}

	/**if ((i < 0) || (i >= strlen)) then RETURN EMPTY_STRING**/
	printf(	"PUSHS TF@$position\n"
			"PUSHS int@0\n"
			"LTS\n");


	char* str = NULL;
	printf("DEFVAR TF@$str_len\n");
	if(string_token->type == ID){
		printf("STRLEN TF@$str_len LF@%s\n", string_token->string);
	}
	else{
		//type == STRING_V
		str = string_convert_constant(string_token->string);
		printf("STRLEN TF@$str_len string@%s\n", str);	
	}
	printf(	"PUSHS TF@$position\n"
			"PUSHS TF@$str_len\n"
			"LTS\n"
			"NOTS\n"
			"ORS\n"
			"PUSHS bool@true\n"
			"JUMPIFEQS $wrong_index_%d\n", call_counter);
	/**********************************************************/



	if(string_token->type == ID){

		printf("STRI2INT TF@&retval_function LF@%s TF@$position\n", string_token->string);
	}
	else{
		//type = STRING_V
		printf("STRI2INT TF@&retval_function string@%s TF@$position\n", str);
	}

	printf("LABEL $wrong_index_%d\n", call_counter);

	call_counter++;
}

void codegen_buildin_chr(TToken* t, int convert_param){

	printf(	"CREATEFRAME\n"
			"DEFVAR TF@&retval_function\n"	//chova se jako by byla funkci
			"DEFVAR TF@$value\n");

	if(t->type == ID){

		printf("MOVE TF@$value LF@%s\n", t->string);
		if(convert_param == DOUBLE2INT) printf("FLOAT2R2OINT TF@$value TF@$value\n");
		printf("INT2CHAR TF@&retval_function TF@$value\n");
	}
	else if(t->type == INT_V){
		//type == INT_V
		printf(	"MOVE TF@$value int@%d\n"
				"INT2CHAR TF@&retval_function int@%d\n", t->int_v, t->int_v);	
	}
	else if(t->type == FLOAT_V){
		//FLOAT_V
		printf(	"MOVE TF@$value float@%g\n"
				"FLOAT2R2OINT TF@$value TF@$value\n"
				"INT2CHAR TF@&retval_function TF@$value\n", t->float_v);	

	}
}

void codegen_buildin_substr(TToken* string_token, TToken* beg_token, TToken* len_token, int convert_param2, int convert_param3){

	static int call_counter = 0;	

	printf(	"CREATEFRAME\n"
			"DEFVAR TF@&retval_function\n"		//chova se jako by byla funkci
			"MOVE TF@&retval_function string@\n"
			
			"DEFVAR TF@*input\n");
	if(string_token->type == ID){
		printf("MOVE TF@*input LF@%s\n", string_token->string);
	}
	else{
		//STRING_V
		char* str;
		str = string_convert_constant(string_token->string);
		printf("MOVE TF@*input string@%s\n", str);
		free(str);
	}

	printf("DEFVAR TF@*index\n");
	if(beg_token->type == ID){
		printf("MOVE TF@*index LF@%s\n", beg_token->string);
		if(convert_param2 == DOUBLE2INT) printf("FLOAT2R2OINT TF@*index TF@*index\n");
	}
	else if(beg_token->type == INT_V){
		//type == INT_V 
		printf("MOVE TF@*index int@%d\n", beg_token->int_v);
	}
	else{
		//FLOAT_V
		printf("MOVE TF@*index float@%g\n", beg_token->float_v);	
		if(convert_param2 == DOUBLE2INT) printf("FLOAT2R2OINT TF@*index TF@*index\n");
	}

	printf("DEFVAR TF@*out_len\n");
	if(len_token->type == ID){
		printf("MOVE TF@*out_len LF@%s\n", len_token->string);
		if(convert_param3 == DOUBLE2INT) printf("FLOAT2R2OINT TF@*out_len TF@*out_len\n");
	}
	else if(len_token->type == INT_V){
		//type == INT_V nebo FLOAT_V
		printf("MOVE TF@*out_len int@%d\n", len_token->int_v);
	}
	else{
		//FLOAT_V
		printf("MOVE TF@*out_len float@%g\n", len_token->float_v);
		if(convert_param3 == DOUBLE2INT) printf("FLOAT2R2OINT TF@*out_len TF@*out_len\n");	
	}

	printf(	"DEFVAR TF@*str_len\n"
			"STRLEN TF@*str_len TF@*input\n"	//delka vstupniho retezce
			
			"PUSHS TF@*str_len\n"
			"PUSHS int@0\n"
			"EQS\n"			//strlen == 0
			
			"PUSHS TF@*index\n"
			"PUSHS int@0\n"
			"GTS\n"
			"NOTS\n"	// i <= 0
			
			"ORS\n"		//strlen == 0 || i <= 0	
			
			"PUSHS bool@true\n"
			"JUMPIFEQS $*end_chr_function_%d\n" //if(strlen == 0 || i <= 0) jump LABEL END

			"PUSHS TF@*out_len\n"
			"PUSHS int@0\n"
			"LTS\n"		//n < 0
			
			"PUSHS TF@*out_len\n"
			"PUSHS TF@*str_len\n"
			"PUSHS TF@*index\n"
			"SUBS\n"
			"GTS\n"	// n > (strlen - i)
			
			"ORS\n"	//n < 0 || n > (strlen - i)
			
			"PUSHS bool@false\n"
			"JUMPIFEQS $*else_branch_%d\n" //if( n < 0 || n > (strlen - i)) then

				"PUSHS TF@*str_len\n"
				"PUSHS TF@*index\n"
				"SUBS\n"
				"PUSHS int@1\n"
				"ADDS\n"
				"POPS TF@*out_len\n"	//n = strlen - i + 1

			"LABEL $*else_branch_%d\n"		//end if

			"SUB TF@*index TF@*index int@1\n"	//i--
			
			"DEFVAR TF@*end_index\n"
			"PUSHS TF@*index\n"
			"PUSHS TF@*out_len\n"
			"ADDS\n"
			"POPS TF@*end_index\n"

			"DEFVAR TF@*state_loop\n"
			"DEFVAR TF@*tmp_char\n", call_counter, call_counter, call_counter);

	/**WHILE LOOP**/
	printf("LABEL $*loop_top_%d\n"
			"LT TF@*state_loop TF@*index TF@*end_index\n"
			"JUMPIFEQ $*end_loop_%d TF@*state_loop bool@false\n"		//while (index <= end_index)
			
				"GETCHAR TF@*tmp_char TF@*input TF@*index\n"
				"CONCAT TF@&retval_function TF@&retval_function TF@*tmp_char\n"	//ret = ret = char[index]
			
			"ADD TF@*index TF@*index int@1\n"	//i++
			"JUMP $*loop_top_%d\n"
			"LABEL $*end_loop_%d\n"//end loop
			"LABEL $*end_chr_function_%d\n", call_counter, call_counter, call_counter, call_counter, call_counter);

	call_counter++;
}

/****PREVOD RETEZCU********************************************************************************************/

//prevede retezec na vystup. format
char* string_convert_constant(char* source){

	//int src_len = strlen(source);
	int out_len = 0;
	int i;
	for(i = 0; source[i] != '\0'; i++){
		if((source[i] >= 0 && source[i] <= 32) || (source[i] == 92))
			if((source[i] == 92) && ((source[i+1] == 'n') || (source[i+1] == '"') || (source[i+1] == '\\') || (source[i+1] == 't'))){
				//povolene escape sekvence IFJ17
				out_len+=4;
				i++;
			}
			else if((source[i] == 92) && isdigit(source[i+1]) && isdigit(source[i+2]) && isdigit(source[i+3])){
				//escape sekvence - ASCII hodnota
				out_len+=4;
				i+=3;
			}
			else{
				out_len+= 4;
			}
		else
			//normalni znak
			out_len++;
	}
	out_len++; //pro '\0'

	
	char* output = NULL;
	output = malloc(sizeof(char)*(out_len+1));

	for(int i = 0, j = 0; source[i] != '\0' ; i++, j++){
		if((source[i] >= 0 && source[i] <= 32) || (source[i] == 92)){
			if(source[i] == 92){
				//mozna escape sekvence
				if(source[i+1] == 'n'){
					strcpy(&output[j],"\\");
					strcpy(&output[j+1],"0");
					strcpy(&output[j+2],"1");
					strcpy(&output[j+3],"0");
					j+=3;
					i++; //preskocit jeden znak navic

				}
				else if(source[i+1] == '"'){
					strcpy(&output[j],"\\");
					strcpy(&output[j+1],"0");
					strcpy(&output[j+2],"3");
					strcpy(&output[j+3],"4");
					j+=3;
					i++; //preskocit jeden znak navic
				}
				else if(source[i+1] == 't'){
					strcpy(&output[j],"\\");
					strcpy(&output[j+1],"0");
					strcpy(&output[j+2],"0");
					strcpy(&output[j+3],"9");
					j+=3;
					i++; //preskocit jeden znak navic
				}
				else if(source[i+1] == '\\'){
					strcpy(&output[j],"\\");
					strcpy(&output[j+1],"0");
					strcpy(&output[j+2],"9");
					strcpy(&output[j+3],"2");
					j+=3;
					i++; //preskocit jeden znak navic
				}
				else if((source[i+1] >= '0') && (source[i+1] <= '9') &&
						(source[i+2] >= '0') && (source[i+2] <= '9') &&
						(source[i+3] >= '0') && (source[i+3] <= '9')){
					//escape ASCII hodnota
					strcpy(&output[j],"\\");

				}
			}
			else{
			strcpy(&output[j],"\\");
			strcpy(&output[j+1],"0");

			char* tmp = NULL;
			if((tmp = malloc(sizeof(char)*3)) == NULL)
				exit(99);
			sprintf(tmp, "%02d", source[i]);
			memcpy(&output[j+2], &tmp[0], sizeof(char));
			memcpy(&output[j+3], &tmp[1], sizeof(char));
			free(tmp);
			j+=3;
			}
		}
		else{
			memcpy(&output[j], &source[i], sizeof(char));
		}		
	}

	output[out_len-1] = '\0';
	return output;
}

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
				printf(	"CREATEFRAME\n"
						"DEFVAR TF@$tmp1l\n"
						
						"POPS TF@$tmp1l\n"
						"INT2FLOATS\n"
						"PUSHS TF@$tmp1l\n");
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
						printf(	"CREATEFRAME\n"
								"DEFVAR TF@$tmp1_conc\n"
								"DEFVAR TF@$tmp2_conc\n"
								"DEFVAR TF@$tmp_res_conc\n"
								
								"POPS TF@$tmp1_conc\n"
								"POPS TF@$tmp2_conc\n"
								"CONCAT TF@$tmp_res_conc TF@$tmp2_conc TF@$tmp1_conc\n"
								"PUSHS TF@$tmp_res_conc\n");
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
					printf(	"CREATEFRAME\n"
							"DEFVAR TF@$tmp1_mul\n"
							
							"INT2FLOATS\n"
							"POPS TF@$tmp1_mul\n"
							"INT2FLOATS\n"
							"PUSHS TF@$tmp1_mul\n"
							"DIVS\n"
							"FLOAT2INTS\n");
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

	printf(	"CREATEFRAME\n"
			"DEFVAR TF@expr\n"
			"POPS TF@expr\n"
			"WRITE TF@expr\n"
			);
}

//vytvori LF
void codegen_scope(){

	printf(	"LABEL &&&scope\n"
			"CREATEFRAME\n"
			"PUSHFRAME\n");

}


/****FUNKCE, DEFINICE A VOLANI**********************************************************************/

void codegen_func_call(char* name, int act_call){

	printf("CALL &func&%s\n", name);
	printf("DEFVAR LF@&&function_return_%d\n", act_call);
	printf("POPS LF@&&function_return_%d\n", act_call);
}

void codegen_get_func_result(int act_call){

	printf("DEFVAR LF@&&function_return_%d\n", act_call);
	printf("POPS LF@&&function_return_%d\n", act_call);
}

//vytvori navesti, prevede TF na LF
void codegen_func_definition(TToken* t){

	printf(	"JUMP &&&scope\n"
			"LABEL &func&%s\n"
			"CREATEFRAME\n"
			"PUSHFRAME\n"
			, t->string);
}

//vlozi do navratove hodnoty implicitni podle nav. typu, pro pripad, ze se ve funkci nenachazi return
void codegen_implicit_func_return(TToken* t){

	if(t->type == INTEGER){
		printf("PUSHS int@0\n");
	}
	else if(t->type == DOUBLE){
		printf("PUSHS float@0.0\n");
	}
	else{
		//t->type == STRING
		printf("PUSHS string@\n");

	}

}

//zkopiruje skutecny parametr do formalniho na LF
void codegen_func_param(char* name){

	printf(	"DEFVAR LF@%s\n"
			"POPS LF@%s\n", name, name);
}

//na konci tela funkce
void codegen_func_return(){

	printf(	"POPFRAME\n"
			"RETURN\n");
}


//pokud se uvnitr funkce nachazi klicove slovo return 
void codegen_func_return_inner(){

	printf("ADDS\n"); //pricte k navratove hodnote implicitni, tj. 0 / 0.0 / !"", tzn. smaze implicitni

	printf(	"POPFRAME\n"
			"RETURN\n");
}

/****IF THEN ELSE STATEMENT************************************************************************/

//skok na else vetev, bool hodnota vyhodnoceni vyrazu je na vrcholu datoveho zasobniku
void codegen_if_cond_jump(int actual_if_id){

	printf(	"PUSHS bool@true\n"
			"JUMPIFNEQS $else_branch_%d\n"
			, actual_if_id);
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
}

void codegen_loop_end(int actual_loop_id){

	printf(	"JUMP $loop_top_%d\n"
			"LABEL $loop_end_%d\n", actual_loop_id, actual_loop_id);
}


/****BUILD IN FUNKCE*********************************************************************************/

void codegen_buildin_length(){

	printf(	"CREATEFRAME\n"
			"PUSHFRAME\n"
			"DEFVAR LF@&result\n"
			"DEFVAR LF@&in\n"
			"POPS LF@&in\n"

				"STRLEN LF@&result LF@&in\n"
				"PUSHS LF@&result\n"
	
			"POPFRAME\n");
}


void codegen_buildin_asc(){

	static int call_counter = 0;

	printf(	"CREATEFRAME\n"
			"PUSHFRAME\n"
			"DEFVAR LF@&result\n"			
			"MOVE LF@&result int@0\n"
			
			"DEFVAR LF@$position\n"
			"POPS LF@$position\n"
			"DEFVAR LF@$input\n"
			"POPS LF@$input\n");
		
			printf("SUB LF@$position LF@$position int@1\n");	//indexovani od 1, ne od nuly
	

	/**if ((i < 0) || (i >= strlen)) then RETURN EMPTY_STRING**/
	printf(	"PUSHS LF@$position\n"
			"PUSHS int@0\n"
			"LTS\n");

	printf(	"DEFVAR LF@$str_len\n"
			"STRLEN LF@$str_len LF@$input\n");

	printf(	"PUSHS LF@$position\n"
			"PUSHS LF@$str_len\n"
			"LTS\n"
			"NOTS\n"
			"ORS\n"
			"PUSHS bool@true\n"
			"JUMPIFEQS $wrong_index_%d\n", call_counter);
	/**********************************************************/

	printf(	"STRI2INT LF@&result LF@$input LF@$position\n");

	printf(	"LABEL $wrong_index_%d\n"
			"PUSHS LF@&result\n"
			"POPFRAME\n"
			,call_counter);

	call_counter++;
}

void codegen_buildin_chr(){

	printf(	"CREATEFRAME\n"
			"PUSHFRAME\n"
			"DEFVAR LF@&result\n"
			"DEFVAR LF@$value\n"
			"POPS LF@$value\n"

			"INT2CHAR LF@&result LF@$value\n"
			"PUSHS LF@&result\n"
			
			"POPFRAME\n");
}

void codegen_buildin_substr(){

	static int call_counter = 0;	

	printf(	"CREATEFRAME\n"
			"PUSHFRAME\n"
			"DEFVAR LF@&result\n"
			"MOVE LF@&result string@\n"
			
			"DEFVAR LF@out_len\n"
			"POPS LF@out_len\n"

			"DEFVAR LF@index\n"
			"POPS LF@index\n"
	
			"DEFVAR LF@input\n"				//cteni parametru
			"POPS LF@input\n"

			"DEFVAR LF@str_len\n"
			"STRLEN LF@str_len LF@input\n"	//delka vstupniho retezce
			
			"PUSHS LF@str_len\n"
			"PUSHS int@0\n"
			"EQS\n"			//strlen == 0
			
			"PUSHS LF@index\n"
			"PUSHS int@0\n"
			"GTS\n"
			"NOTS\n"	// i <= 0
			
			"ORS\n"		//strlen == 0 || i <= 0	
			
			"PUSHS bool@true\n"
			"JUMPIFEQS $*end_chr_function_%d\n" //if(strlen == 0 || i <= 0) jump LABEL END

			"PUSHS LF@out_len\n"
			"PUSHS int@0\n"
			"LTS\n"		//n < 0
			
			"PUSHS LF@out_len\n"
			"PUSHS LF@str_len\n"
			"PUSHS LF@index\n"
			"SUBS\n"
			"GTS\n"	// n > (strlen - i)
			
			"ORS\n"	//n < 0 || n > (strlen - i)
			
			"PUSHS bool@false\n"
			"JUMPIFEQS $*else_branch_%d\n" //if( n < 0 || n > (strlen - i)) then

				"PUSHS LF@str_len\n"
				"PUSHS LF@index\n"
				"SUBS\n"
				"PUSHS int@1\n"
				"ADDS\n"
				"POPS LF@out_len\n"	//n = strlen - i + 1

			"LABEL $*else_branch_%d\n"		//end if

			"SUB LF@index LF@index int@1\n"	//i--
			
			"DEFVAR LF@end_index\n"
			"PUSHS LF@index\n"
			"PUSHS LF@out_len\n"
			"ADDS\n"
			"POPS LF@end_index\n"

			"DEFVAR LF@state_loop\n"
			"DEFVAR LF@tmp_char\n", call_counter, call_counter, call_counter);

	/**WHILE LOOP**/
	printf("LABEL $*loop_top_%d\n"
			"LT LF@state_loop LF@index LF@end_index\n"
			"JUMPIFEQ $*end_loop_%d LF@state_loop bool@false\n"		//while (index <= end_index)
			
				"GETCHAR LF@tmp_char LF@input LF@index\n"
				"CONCAT LF@&result LF@&result LF@tmp_char\n"	//ret = ret = char[index]
			
			"ADD LF@index LF@index int@1\n"	//i++
			"JUMP $*loop_top_%d\n"
			"LABEL $*end_loop_%d\n"//end loop
			"LABEL $*end_chr_function_%d\n", call_counter, call_counter, call_counter, call_counter, call_counter);


		printf("PUSHS LF@&result\n");
		printf("POPFRAME\n");
	
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

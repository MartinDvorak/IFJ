
#include "codegen.h"

/****ZPRACOVANI VYRAZU****************************************************************/

//prida prvek do pole operandu 
void insert_operand_array(TToken* t, TExpr_operand* operand_array, int* ptr_to_array){

	TExpr_operand tmp;

	if(t->type == INT_V){
		//int konstanta
		tmp.semantic_type = INTEGER;
		tmp.type = INT_V;
		tmp.int_v = t->int_v;
		tmp.name = NULL;
		tmp.string = NULL;
	}
	else if(t->type == FLOAT_V){
		//float konstanta
		tmp.semantic_type = DOUBLE;
		tmp.type = FLOAT_V;
		tmp.float_v = t->float_v;
		tmp.name = NULL;
		tmp.string = NULL;
	}
	else if(t->type == STRING_V){
		//string konstanta
		tmp.semantic_type = STRING;
		tmp.type = STRING_V;
		if((tmp.string =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
		}
		memcpy(tmp.string, t->string, strlen(t->string));
		tmp.name = NULL;
	}
	else if(t->type == ID){
		//promenna
		Tdata* symbTmp = NULL;
		search_tree(root_local,t->string,symbTmp);
		if(symbTmp->type == INTEGER){
			//promenna integer
			tmp.semantic_type = INTEGER;
			tmp.type = INTEGER;
			if((tmp.name =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
			}
			memcpy(tmp.name,t->string, strlen(t->string));
			tmp.string = NULL;
		}
		else if(symbTmp->type == DOUBLE){
			//promenna float
			tmp.semantic_type = DOUBLE;
			tmp.type = DOUBLE;
			if((tmp.name =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
			}
			memcpy(tmp.name,t->string, strlen(t->string));
			tmp.string = NULL;
		}
		else if(symbTmp->type == STRING){
			//promenna string
			tmp.semantic_type = STRING;
			tmp.type = STRING;
			if((tmp.name =  malloc(sizeof(char)*(strlen(t->string)+1))) == NULL){
			exit(99);
			}
			memcpy(tmp.name,t->string, strlen(t->string));
			tmp.string = NULL;
		}
	}

	//pridani do pole operandu
	operand_array[*ptr_to_array++] = tmp;
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
void codegen_expression(TExpr_operand* operand_array, char* postfix, Toperation* op_arr, int* expr_ret_id){
	
	int operand_index = 0;	//kolik operandu v poli se uz proslo
	int operator_index = 0;	//kolik operatoru v poli se uz proslo

	static int ret_id = 1; 	//jednoznacny identifikator pro vysledek vyrazu
						//kazdy vysledek bude ve tvaru LF@%retval_expr[ret_id], pozn. bez []

	for(int i = 0; postfix[i] != '\0'; i++){

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
				printf("CREATEFRAME\n");  //vycisteni TF
			}
			if(act.r_convert){
				//pravy operand konverze
				printf("CREATEFRAME\n");
				printf("DEFVAR TF@$tmp1\n");

				printf("POPS TF@$tmp1\n");
				printf("INT2FLOATS\n");
				printf("PUSHS TF@$tmp1\n");
				printf("CREATEFRAME\n");  //vycisteni TF
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
						printf("CREATEFRAME\n");
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
			//konec vyrazu, ulozeni vysledku vyrazu
			printf("DEFVAR LF@$expr_retval%d\n", ret_id);
			printf("POPS LF@$expr_retval%d\n", ret_id);
			printf("CLEARS\n");
			*expr_ret_id = ret_id;
			ret_id++; //inkrement unique id navratove hodnoty
		}
	}
}


/****KONEC ZPRACOVANI VYRAZU**********************************************************/



//zatim jen prototyp
void codegen_Dim(TToken* t){

	Tdata* tdata = NULL;

	search_tree(root_local, t->string, tdata);

	printf("DEFVAR LF@%s\n", t->string);

	int type = tdata->type;

	switch(type){

		case INTEGER:
			printf("MOVE LF@%s int@0", t->string);
			break;
		case DOUBLE: 
			printf("MOVE LF@%s float@0.0", t->string);
			break;
		case STRING:
			printf("MOVE LF@%s string@\"\"", t->string);
			break;
	}
}


void codegen_scope(){

	printf("CREATEFRAME\n");

}

void codegen_input(TToken* t){

	Tdata* data = NULL;
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


void codegen_print(int expr_ret_id){

	printf("WRITE LF@expr_retval%d\n", expr_ret_id);
}

/****POMOCNE*****************************************************************************/

//prevede retezec na vystup. format; prototyp, nevim jiste jestli funguje dobre + nejakej leak
char* string_convert_constant(char* source){

	int out_len = 0;

	for(int i = 0; source[i] != '\0'; i++){
		//delka vystupniho retezce
		if(((source[i] >= 0) && (source[i] <= 32)) || (source[i] == 92))
			out_len+=4;
		else
			out_len++;
	}

	char* out;
	if((out = malloc(sizeof(char)*(out_len+1))) == NULL){
		exit(99);
	}

	for(int i = 0, j = 0; source[i] != '\0'; i++, j++){
		//kopirovani znaku
		if(((source[i] >= 0)  && (source[i] <= 32)) || (source[i] == 92)){
			//escape sekvence
			out[j] = '\\';
			out[j+1] = '0';

			char* tmp = NULL;
			if((tmp = malloc(sizeof(char)*3)) == NULL){
				exit(99);
			}

			sprintf(tmp, "%02d", source[i]);
			out[j+2] = tmp[0];
			out[j+3] = tmp[1];
			j+=3;

			free(tmp);		
		}
		else{
			//normalni znak
			out[j] = source[i];
		}
	}

	return out;
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
		printf("STR2INT TF@$char_no TF@$input \n");	//zjisteni ASCII znaku


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
	printf("CREATEFRAME\n");
}
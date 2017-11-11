
#include "codegen.h"


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



void codegen_expression(TExpr_operand* operand_array, char* postfix, Toperation* op_arr){
	
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
						str_out = string_convert(operand_array[operand_index].string);				
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
				if(act.op == 'M'){
					//konverze na int
					printf("CREATEFRAME\n");
					printf("DEFVAR TF@$tmp1\n");
					printf("DEFVAR TF@$tmp2\n");
					printf("POPS TF@$tmp1\n");
					printf("POPS TF@$tmp2\n");
					printf("FLOAT2REINTS\n");
					printf("PUSHS TF@$tmp2\n");
					printf("PUSHS TF@$tmp1\n");
					printf("CREATEFRAME\n");  //vycisteni TF
				}
				else{
					//ve vsech ostatnich pripadech konverze na float
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
			}
			if(act.r_convert){
				//pravy operand konverze
				if(act.op == 'M'){
					//konverze na int
					printf("CREATEFRAME\n");
					printf("DEFVAR TF@$tmp1\n");
					printf("POPS TF@$tmp1\n");
					printf("FLOAT2REINTS\n");
					printf("PUSHS TF@$tmp1\n");
					printf("CREATEFRAME\n");  //vycisteni TF
				}
				else{
					//ve vsech ostatnich pripadech konverze na float
					printf("CREATEFRAME\n");
					printf("DEFVAR TF@$tmp1\n");
					printf("POPS TF@$tmp1\n");
					printf("INT2FLOATS\n");
					printf("PUSHS TF@$tmp1\n");
					printf("CREATEFRAME\n");  //vycisteni TF
				}
			}

			switch(act.op){
				//provedeni operace
				case '+':
					printf("ADDS\n");
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
					//TODO modulo
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
			printf("POPS LF@$expr_retval\n");
			ret_id++; //inkrement unique id navratove hodnoty
		}
	}
}





/*****************************************************************************************************/

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


//prevede retezec na vystup. format; prototyp, nevim jiste jestli funguje dobre + nejakej leak
char* string_convert(char* source){

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
#include "parser.h"
#include "symbtab.h"
#include "expr.h"
#include "token.h"
#include "semantic.h"

Ttnode_ptr root_global = NULL;
Ttnode_ptr root_local = NULL;
int return_type = 0;


Tstack* LA_S = NULL;
int storage = -2;

int ERROR_RETURN = 2;


int preprocesing_expr(TToken* t, TToken *last, int condition, int* exp_ret)
{ // TODO - expresion
	char* string;
	if ((string = malloc(sizeof(char)*10)) == NULL)
		exit(99);
	string = strcpy(string,"");

	TExpr_operand *operand_array;
	int ptr_to_array = 0;
	if ((operand_array = malloc(sizeof(struct expr_operand)*10)) == NULL)
		exit(99);
	
	if (last != NULL)
	{
		insert_operand_array(last, operand_array, &ptr_to_array);

		string = strcat(string, "i");
	}
	while(TRUE)
	{

		if((t->type == EOL) || (t->type == SEMICOLON) || (t->type == THEN))
			break;
		
		if(strlen(string)%10 == 9)
		{
			int size = strlen(string)%10;
			if ((string = realloc(string, sizeof(char)*(size+11))) == NULL)
				exit(99);
		}

		if(ptr_to_array%10 == 9)
		{
			if ((operand_array = realloc(operand_array, sizeof(struct expr_operand)*(ptr_to_array+10))) == NULL)
				exit(99);
		}

		if(condition)
		{
		switch(t->type)
		{
			case MUL: string = strcat(string, "*");
					break;
			case DIV:string = strcat(string, "/");
					break;
			case ADD:string = strcat(string, "+");
					break;
			case SUB:string = strcat(string, "-");
					break;
			case INTDIV:string = strcat(string, "M");
					break;
			case ID: if(!semantic_find_id(t)) 
						{
							free(string);
							operand_array_destructor(operand_array, ptr_to_array);
							return FALSE;
						}
			case INT_V: 
			case FLOAT_V:
			case STRING_V:
					// TODO pro komplikovanejsi strukturu predelat insert
					insert_operand_array(t, operand_array, &ptr_to_array);
					string = strcat(string, "i");
					break;
			case BRACKET_L: string = strcat(string, "(");
					break;
			case BRACKET_R: string = strcat(string, ")");
					break;
			
			case ASSIGN:string = strcat(string, "=");
					break;
			case NEQ: string = strcat(string, "N");
					break;
			case LESS:string = strcat(string, "L");
					break;
			case GREAT:string = strcat(string, "G");
					break;
			case LESSEQ:string = strcat(string, "S");
					break;
			case GREATEQ:string = strcat(string, "R");
					break;
			default:
				free(string);
				operand_array_destructor(operand_array, ptr_to_array);
				return FALSE;
		}
		}
		else{
		switch(t->type)
		{	
			case MUL: string = strcat(string, "*");
					break;
			case DIV:string = strcat(string, "/");
					break;
			case ADD:string = strcat(string, "+");
					break;
			case SUB:string = strcat(string, "-");
					break;
			case INTDIV:string = strcat(string, "M");
					break;
			case ID: if(!semantic_find_id(t)) 
						{
							free(string);
							operand_array_destructor(operand_array, ptr_to_array);
							return FALSE;
						}
			case INT_V:
			case FLOAT_V:
			case STRING_V:
					// TODO pro komplikovanejsi strukturu predelat insert
					insert_operand_array(t, operand_array, &ptr_to_array);
					string = strcat(string, "i");
					break;
			case BRACKET_L: string = strcat(string, "(");
					break;
			case BRACKET_R: string = strcat(string, ")");
					break;
			default:
				free(string);
				operand_array_destructor(operand_array, ptr_to_array);
				return FALSE;	
			
		}
		}	
		t = get_next(t,LA_S,&storage);
	} // indikuje konec vyrazu 
	string = strcat(string, "$");

	Toperation* op_arr = NULL;
	int num_of_op = 0;

	if((op_arr = malloc(sizeof(struct semantic_operation)*(ptr_to_array))) == NULL)
		exit(99);
	
	int res = FALSE;
	char* postfix;

	if((postfix = malloc(sizeof(char)*(strlen(string)))) == NULL)
		exit(99);
	postfix = strcpy(postfix,"");	

	if(expr(string,condition,postfix) && semantic_exp(postfix,operand_array,op_arr,&num_of_op,exp_ret))
		{
		res = TRUE;

		/*****GENEROVANI MEZIKODU****************************************/
		codegen_expression(operand_array, postfix, op_arr);
		}
	

	free(string);
	operand_array_destructor(operand_array, ptr_to_array);
	free(op_arr);
	free(postfix);

	return res;		


}

int expr_n(TToken *t)
{
	int nul; // zahodi se
	if (t->type == EOL)
		return TRUE;
	else if (preprocesing_expr(t,NULL,0,&nul))	
	{
		/*****GENEROVANI MEZIKODU****************************************/
		codegen_print();


		if (t->type == SEMICOLON)
		{
			t = get_next(t,LA_S,&storage);
			return expr_n(t);
		}	
	}
	return FALSE;	
}

int build_in_fce(TToken *t)
{
	TToken tmp;

		if(t->type == LENGTH)
		{ // Length (s as string) as integer
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID)||(t->type == INT_V)||(t->type == FLOAT_V)||(t->type == STRING_V))
				{
					tmp = *t;
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s', NULL))
						return FALSE;
					//end
					t = get_next(t,LA_S,&storage);
					if(t->type == BRACKET_R)
					{
						/****GENEROVANI MEZIKODU*************/
						codegen_buildin_length(&tmp);

						t = get_next(t,LA_S,&storage);
						return TRUE;
					}
				}
			}
		}
		else if(t->type == SUBSTR)
		{ // Substr(s as string, i as integer, n as integer) as STRING
			TToken tmp2;
			TToken tmp3;
			int convert_param2;
			int convert_param3;
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID)||(t->type == INT_V)||(t->type == FLOAT_V)||(t->type == STRING_V))
				{

					/**GENEROVANI MEZIKODU*************************/
					tmp.type = t->type;
					tmp.int_v = t->int_v;
					tmp.float_v = t->float_v;
					if((tmp.string = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
						exit(99);
					strcpy(tmp.string, t->string);

					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s', NULL))
						return FALSE;
					//
					t = get_next(t,LA_S,&storage);
					if(t->type == COLON)
					{
						t = get_next(t,LA_S,&storage);
						if((t->type == ID)||(t->type == INT_V)||(t->type == FLOAT_V)||(t->type == STRING_V))
						{
							/**GENEROVANI MEZIKODU*************************/
							tmp2.type = t->type;
							tmp2.int_v = t->int_v;
							tmp2.float_v = t->float_v;
							if((tmp2.string = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
								exit(99);
							strcpy(tmp2.string, t->string);

							// TODO sementicka kontrola typu
							if(!semantic_id(root_local,t,'i', &convert_param2))
								return FALSE;
							//
							t = get_next(t,LA_S,&storage);
							if(t->type == COLON)
							{
								t = get_next(t,LA_S,&storage);
								if((t->type == ID)||(t->type == INT_V)||(t->type == FLOAT_V)||(t->type == STRING_V))
								{
									/**GENEROVANI MEZIKODU*************************/
									tmp3.type = t->type;
									tmp3.int_v = t->int_v;
									tmp3.float_v = t->float_v;
									if((tmp3.string = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
										exit(99);
									strcpy(tmp3.string, t->string);


									// TODO sementicko kontrolu dat typu
									if(!semantic_id(root_local,t,'i', &convert_param3))
										return FALSE;
									//
									t = get_next(t,LA_S,&storage);
									if(t->type == BRACKET_R)
									{

										/****GENEROVANI MEZIKODU****************/
										codegen_buildin_substr(&tmp, &tmp2, &tmp3, convert_param2, convert_param3);
										free(tmp.string);
										free(tmp2.string);
										free(tmp3.string);



										t = get_next(t,LA_S,&storage);
										return TRUE;
									}
								}
							}
						}
					}
					
				}
			}
			
		}
		else if(t->type == ASC)
		{ // Asc(s as string, i as integer) as integer
			int convert_param = 0;
			TToken tmp2; //save position
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID)||(t->type == INT_V)||(t->type == FLOAT_V)||(t->type == STRING_V))
				{

					tmp = *t;
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s', NULL))
						return FALSE;
					//end
					t = get_next(t,LA_S,&storage);
					if(t->type == COLON)
					{
						t = get_next(t,LA_S,&storage);
						if((t->type == ID)||(t->type == INT_V)||(t->type == FLOAT_V)||(t->type == STRING_V))
						{	
							
							tmp2 = *t;
							// TODO sementicka kontrola typu
							if(!semantic_id(root_local,t,'i', &convert_param))
								return FALSE;
							//
							t = get_next(t,LA_S,&storage);
							if(t->type == BRACKET_R)
							{	

								/****GENEROVANI MEZIKODU**************/
								codegen_buildin_asc(&tmp, &tmp2, convert_param);

								t = get_next(t,LA_S,&storage);
								return TRUE;
							}
						}
					}
					
				}
			}
			
		}
		else if(t->type == CHR)
		{ // Chr(i as integer) as string
			int convert_param;
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID)||(t->type == INT_V)||(t->type == FLOAT_V)||(t->type == STRING_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'i', &convert_param))
						return FALSE;
					//
					t = get_next(t,LA_S,&storage);
					if(t->type == BRACKET_R)
					{

						/****GENEROVANI MEZIKODU*******************/
						codegen_buildin_chr(&tmp, convert_param);

						t = get_next(t,LA_S,&storage);
						return TRUE;
					}
				}
			}
		}
	return FALSE;	
}

//volani funkce
int param_fn(TToken *t, char* param, int* position)
{
	int convert_param = 0;

	static int param_no = 2; //zacim od indexu 2, index 1 v param_f

	if(t->type == BRACKET_R){
	
		param_no = 2;	//reinicializace, vsechny parametry funkce byly zpracovany
		return TRUE;
	}
	else if(t->type == COLON)
	{
		t = get_next(t,LA_S,&storage);
		if((t->type == ID )||(t->type == FLOAT_V) ||(t->type == INT_V) ||(t->type == STRING_V))
		{
			if(!semantic_id_param(t,param,position, &convert_param))
				return FALSE;

			/****GENEROVANI MEZIKODU*******************/
			codegen_func_call_give_param(t,param_no, convert_param);
			param_no++;

			t = get_next(t,LA_S,&storage);
			return param_fn(t,param,position);
		}
	}
	return FALSE;
}

//volani funkce
int param_f(TToken *t, char* param, int* position)
{
	int convert_param = 0;

	if(t->type == BRACKET_R){
	
		/****GENEROVANI MEZIKODU********************/
		codegen_empty_func_frame();
		return TRUE;
	}
	else if((t->type == ID )||(t->type == FLOAT_V) ||(t->type == INT_V) ||(t->type == STRING_V))
	{
		if(!semantic_id_param(t,param,position, &convert_param))
			return FALSE;

		/****GENEROVANI MEZIKODU*******************/
		codegen_empty_func_frame();
		codegen_func_call_give_param(t,1, convert_param);

		t = get_next(t,LA_S,&storage);
		return(param_fn(t,param,position));
	}
	return FALSE;
}

int r_side(TToken *t,int lvalue, int* r_side_type, int* convert_func_result)
{ // zkontrolovat zda se prava strana rovna typove leve
	int rvalue;
	if(t->type == ID)
	{ // TODO zajistit preprocessing id
		TToken tmp = *t;
		char* f_name;	//ulozeni id funkce
		if((f_name = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
			exit(99);
		strcpy(f_name, t->string);

		t = get_next(t,LA_S,&storage);

		if(t->type == BRACKET_L)
		{ // <r_side> -> id(<param_f>) EOL 
			// TODO kontrolu dat typu
			*r_side_type = R_SIDE_FCALL;
			int position = 0;
			char *param = NULL;
			
			// semantic chceck
			if(!semantic_fce_param(root_global,&tmp,&param))
				return FALSE;
			if(!semantic_id_type(root_global,t,&rvalue))
				return FALSE;
			if(!semantic_check_lside_rside(lvalue,rvalue, *r_side_type, convert_func_result))
				return FALSE;
			semantic_flag_use(&root_global,t);
			/// end
			t = get_next(t,LA_S,&storage);
			if(param_f(t,param,&position))
				if(t->type == BRACKET_R)
				{
					// bacha na off by one
					// TODO AZ Bude semantika od comentovat
					if(position != (int)strlen(param))
						{
							ERROR_RETURN = 4;
							return FALSE;
						}
					
					t = get_next(t,LA_S,&storage);
					if(t->type == EOL)
					{
						/****GENEROVANI MEZIKODU****************/
						//skok na label funkce
						codegen_func_call(f_name);
						free(f_name);

						return TRUE;
					}
				}
		} 
		else if(preprocesing_expr(t,&tmp,0,&rvalue))
		{ // <r_side> -> <expr> EOL
			*r_side_type = R_SIDE_EXPR;

			if(t->type == EOL)
			{
				// 
				if(!semantic_check_lside_rside(lvalue,rvalue, *r_side_type, convert_func_result))
					return FALSE;
				return TRUE;
			}
		}
	}	
	else if ((t->type == LENGTH) || (t->type == SUBSTR) || (t->type == ASC) ||(t->type == CHR) )
	{ // BUILD IN FUNCTION
		// TODO SEMANTICKA KONTROLA
		*r_side_type = R_SIDE_BUILD_IN;

		if(!semantic_check_lside_rside(lvalue,semantico_convert_buildin(t->type),R_SIDE_BUILD_IN, convert_func_result))
			return FALSE;

		if(build_in_fce(t))
		{
			if(t->type == EOL)
			{
				return TRUE;
			}
		}
	}
	// add
	else{
		if(preprocesing_expr(t,NULL,0,&rvalue))
		{ // <r_side> -> <expr> EOL
			*r_side_type = R_SIDE_EXPR;

			if(t->type == EOL)
			{
				// 
				if(!semantic_check_lside_rside(lvalue,rvalue, *r_side_type, convert_func_result))
					return FALSE;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


int equal(TToken *t,int lvalue, int* r_side_type, int* convert_func_result)
{
	if (t->type == EOL)
		return TRUE;
	else if (t->type == ASSIGN)
	{
		t = get_next(t,LA_S,&storage);
		return r_side(t,lvalue, r_side_type, convert_func_result);
	}
	return FALSE;
}	

int body(TToken *t)
{	// <BODY> -> epsilon
	int nul; // pro zahozeni;
	if( (t->type == END) || (t->type == ELSE) || (t->type == LOOP) )
		return TRUE;
	else if(t->type == DIM)
	{ // <BODY> -> DIM ID AS <TYPE> <=> EOL <BODY>
		t = get_next(t,LA_S,&storage);
		if(t->type == ID)
		{	
			char* name = NULL;	//ulozeni jmena fuknce pro codegen
			if((name = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
				exit(99);
			strcpy(name, t->string);

			// TODO - insert to local_tree
			Tdata tmp;
			tmp.defined = 1;
			tmp.param = NULL;	
			t = get_next(t,LA_S,&storage);
			if(t->type == AS)
			{
				t = get_next(t,LA_S,&storage);
				if(type(t,&tmp,1,NULL))
				{	
					// semantic control
					if(!semantic_insert_id(&root_local,root_global,t->string,&tmp))
						return FALSE;
					//
					t = get_next(t,LA_S,&storage);
					int r_side_type;
					r_side_type = R_SIDE_NONE;

					/****GENEROVANI MEZIKODU********************/
					codegen_dim(name);

					int convert_func_result = 0;
					if(equal(t,tmp.type, &r_side_type, &convert_func_result))
					{
						if(t->type == EOL)
						{ 
							/****GENEROVANI MEZIKODU***************************/
							codegen_dim_r_side(name, r_side_type, convert_func_result);
							free(name);

							t = get_next(t,LA_S,&storage);
							return body(t);
						}
					}
				}
			}
		}
	}
	else if(t->type == ID)
	{ // <BODY> -> ID = <R_SIDE> EOL <BODY>
		// TODO semanticky overit 
		char* name = NULL;	//ulozeni jmena fuknce pro codegen
		if((name = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
			exit(99);
		strcpy(name, t->string);

		if(!semantic_find_id(t))
			return FALSE;	
		int lvalue;
		if(!semantic_id_type(root_local,t,&lvalue))
			return FALSE;

		t = get_next(t,LA_S,&storage);
		if (t->type == ASSIGN)
		{
			t = get_next(t,LA_S,&storage);
			int r_side_type;
			int convert_func_result = 0;
			if(r_side(t,lvalue, &r_side_type, &convert_func_result))
			{
				if(t->type == EOL)
				{

					/****GENEROVANI MEZIKODU**********************/
					codegen_assignment(name, r_side_type, convert_func_result);
					free(name);

					t = get_next(t,LA_S,&storage);
					return body(t);
				}
			}
		}
	}
	else if(t->type == INPUT)
	{ // <BODY> -> INPUT ID EOL <BODY>
		t = get_next(t,LA_S,&storage);
		if(t->type == ID)
		{
			/*****GENEROVANI MEZIKODU****************************************/
			codegen_input(t);

			// TODO - semanticka kontrola
			if(!semantic_find_id(t))
				return FALSE;
			//
			t = get_next(t,LA_S,&storage);
			if(t->type == EOL)
			{
				t = get_next(t,LA_S,&storage);
				return body(t);
			}
		}
	}
	else if(t->type == PRINT)
	{ // <BODY> -> PRINT <EXP>; <EXP_N> EOL <BODY>
		t = get_next(t,LA_S,&storage);
		if(preprocesing_expr(t,NULL,0,&nul)){

			/*****GENEROVANI MEZIKODU****************************************/
			codegen_print();

			if(t->type == SEMICOLON)
			{
				t = get_next(t,LA_S,&storage);
				if(expr_n(t))
					if(t->type == EOL)
					{
						t = get_next(t,LA_S,&storage);
						return body(t);
					}
			}
		}
	}
	else if(t->type == IF)
	{ // <BODY> -> IF <EXP> THEN EOL <BODY> ELSE EOL <BODY> END IF EOL <BODY>
		static int unique_if_id = 0;
		int actual_if_id = unique_if_id;	//musim si ulozit lokalne, protoze jinak by me to rekurze inkrementovala
		unique_if_id++;

		t = get_next(t,LA_S,&storage);
		if(preprocesing_expr(t,NULL,1,&nul))
			if (t->type == THEN)
			{
				t = get_next(t,LA_S,&storage);
				if(t->type == EOL)
				{
					/****GENEROVANI MEZIKODU********************/
					codegen_if_cond_jump(actual_if_id);

					t = get_next(t,LA_S,&storage);
					if(body(t))
						if(t->type == ELSE)
						{
							t = get_next(t,LA_S,&storage);
							if(t->type == EOL)
							{
								/****GENEROVANI MEZIKODU*******************/
								codegen_else_label(actual_if_id);

								t = get_next(t,LA_S,&storage);
								if(body(t))
									if(t->type == END)
									{
										t = get_next(t,LA_S,&storage);
										if(t->type == IF)
										{
											t = get_next(t,LA_S,&storage);
											if(t->type == EOL)
											{
												/****GENEROVANI MEZIKODU*****************/
												codegen_if_end_label(actual_if_id);

												t = get_next(t,LA_S,&storage);
												return body(t);
											}
										}
									}
							}
						}
				}
			}
	}
	else if(t->type == DO)
	{ // <BODY> -> DO WHILE <EXP> EOL <BODY> LOOP EOL <BODY>
		static int unique_loop_id = 0;
		int actual_loop_id = unique_loop_id;	//musim si ulozit lokalne, protoze jinak by me to rekurze inkrementovala
		unique_loop_id++;

		t = get_next(t,LA_S,&storage);
		if(t->type == WHILE)
		{

			/****GENEROVANI MEZIKODU***************/
			codegen_loop_top_label(actual_loop_id);

			t = get_next(t,LA_S,&storage);
			if(preprocesing_expr(t,NULL,1,&nul))
				if(t->type == EOL)
				{
					/****GENEROVANI MEZIKODU*****************/
					codegen_loop_cond(actual_loop_id);

					t = get_next(t,LA_S,&storage);
					if(body(t))
						if(t->type == LOOP)
						{
							t = get_next(t,LA_S,&storage);
							if(t->type == EOL)
							{
								/****GENEROVANI MEZIKODU*****************/
								codegen_loop_end(actual_loop_id);

								t = get_next(t,LA_S,&storage);
								return body(t);
							}
						}
				}
		}
	}	
	else if(t->type == RETURN)
	{ // <BODY> -> RETURN <EXP> EOL <BODY>
		// TODO - semanticka kontrolo jestli je return s return typem
		int rvalue;
		t = get_next(t,LA_S,&storage);
		if(preprocesing_expr(t,NULL,0,&rvalue))
		{
			
			if(return_type == 0)
			{	
				return FALSE;
			}
			int convert_func_result = 0;
			if(!semantic_check_lside_rside(return_type,rvalue, FUNC_RETURN, &convert_func_result))
				return FALSE;

			/****GENEROVANI MEZIKODU*************/
			codegen_func_return();

			//end semantic
			if(t->type == EOL)
				{
					t = get_next(t,LA_S,&storage);
					return body(t);
				}
		}
	}
	else if(t->type == EOL)
	{
		t = get_next(t,LA_S,&storage);
		return body(t);
	}

	return FALSE;
}

void add_char_to_param(TToken *t, Tdata *data)
{
	if(data->param == NULL) 
	{
		if((data->param = malloc(sizeof(char)*10)) == NULL)
			exit(99);
		data->param = strcpy(data->param, "");
	}
	else if(strlen(data->param)%10 == 9)
	{
		int size = strlen(data->param)+1; 
		if((data->param = realloc(data->param, sizeof(char)*(size+10))) == NULL)
			exit(99);
	}
	switch(t->type)
	{
		case INTEGER: data->param = strcat(data->param,"i");	
			break;
		case STRING: data->param = strcat(data->param,"s");
			break;
		case DOUBLE: data->param = strcat(data->param,"f");
			break;		
	}
}

int type(TToken* t, Tdata* data, int type, int* to_symbtab)
{
	if ((t->type == INTEGER) || (t->type == STRING) ||(t->type == DOUBLE))
	{
		if(type)
		{
			data->type = t->type;
		}
		else{
			add_char_to_param(t,data);
			*to_symbtab = t->type;
		}
		return TRUE;
	}
	return FALSE;
}

//definice, deklarace funkce
int params_N(TToken *t, Tdata *data, int local)
{
	//cislo, po kolikate je volana v ramci jedne definice --> kolikaty parametr (poprve u 2.)
	static int called_per_function = 2;

	if (t->type == BRACKET_R){
		called_per_function = 2; //konec definice jedne funkce, reinicializace 
		return TRUE;
	}
	else if(t->type == COLON)
	{
		Tdata tmp;
		t = get_next(t,LA_S,&storage);
		if (t->type == ID)
		{
			/****GENEROVANI MEZIKODU**********************/
			if(local){
			codegen_func_param(t, called_per_function);
			called_per_function++;
			}

			t = get_next(t,LA_S,&storage);
			if(t->type == AS)
			{
				t = get_next(t,LA_S,&storage);
				if(type(t,data,0,&(tmp.type)))
				{
					// insert local tree
					if(local)
					{
						tmp.defined = 1;
						tmp.param = NULL;
						if(!semantic_insert_id(&root_local,root_global,t->string,&tmp))
							return FALSE;	
					}
					t = get_next(t,LA_S,&storage);
					if(params_N(t,data,local))
						return TRUE;
				}
			}
		}	
	}
	return FALSE;
}

//definice, deklarace funkce
int param(TToken* t, Tdata* data, int local)
{ //IMPLICITNE budeme predpokladat ze nebudeme menit string 
	if (t->type == ID)
	{   

		/****GENEROVANI MEZIKODU***********************/
		if(local){
			//vzdy prvni parametr
			codegen_func_param(t,1);
		}

		Tdata tmp;
		t = get_next(t,LA_S,&storage);
		if(t->type == AS)
		{
			t = get_next(t,LA_S,&storage);
			if(type(t,data,0,&(tmp.type)))
			{	
				//insert local tree
				if(local)
				{
					tmp.defined = 1;
					tmp.param = NULL;
					if(!semantic_insert_id(&root_local,root_global,t->string,&tmp))
						return FALSE;
				}
				//
				t = get_next(t,LA_S,&storage);
				if(params_N(t,data,local))
					return TRUE;
			}
		}
	}
	else if(t->type == BRACKET_R)
	{
		if((data->param = malloc(sizeof(char)*2)) == NULL)
			exit(99);
		data->param = strcpy(data->param, "");
		return TRUE;
	}
	return FALSE;
}

int func_line(TToken* t,int local)
{ // TODO - pres defined // local / global tree ? 
	Tdata tmp;
	tmp.defined = local;
	tmp.param = NULL;

	int flag = 0;
	if(t->type == FUNCTION)		
	{ //func_line -> function id (<param>) as <type> eol
		t = get_next(t,LA_S,&storage);
		if(t->type == ID)
		{	
			// sementic
			if(local)
			{
				int prom = semantic_check_define(&root_global,t->string);

				/*****GENEROVANI MEZIKODU*************************/
					codegen_func_definition(t);

				if(prom == 0)// redefinace
					return FALSE;
				else if(prom == -1){ // prvni def
					if(!semantic_insert(&root_global, t->string, &tmp))
						return FALSE; // neni prvni def					//
				}
				else{
					flag = 1;
				}
			}
			else{
				if(!semantic_insert(&root_global, t->string, &tmp))
					return FALSE;
			}
			//
			char *name =NULL;
			if ((name = malloc(sizeof(char)*strlen(t->string))) == NULL)
				exit(99);
			name = strcpy(name,t->string);
			// semantic
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				//semntika -pokud bude definice fce -delam lokalni strom
				if(local)
				{
					free_tree(&root_local);
				}
				//
				t = get_next(t,LA_S,&storage);
				if(param(t, &tmp,local))
					if(t->type == BRACKET_R)
					{
						t = get_next(t,LA_S,&storage);
						if(t->type == AS)
						{
						 	t = get_next(t,LA_S,&storage);
						 	if(type(t,&tmp,1,NULL))
						 	{
						 		// semantic
						 		if(!semantic_return_type(&return_type,local,tmp.type,name,flag))
						 			return FALSE;
						 		// end  semantic

						 		/**GENEROVANI MEZIKODU**********************/
						 		if(local){
						 		codegen_implicit_func_return(t);
						 		}

						 		t = get_next(t,LA_S,&storage);
						 		if(t->type == EOL)
						 		{
						 			// update semantic
						 			if(!flag)
						 			{
						 				insert_data_tree(&root_global, name, &tmp);
						 			}
						 			else{
						 				if(!semantic_check_params(root_global,name,tmp.param))
						 					return FALSE;
						 				free(tmp.param);
						 			}
						 			free(name);
						 			t = get_next(t,LA_S,&storage);
						 			return TRUE;
						 		}	
						 	}
						}
					}
			}
			free(name);
		}

	}
	return FALSE;
}
int func(TToken* t, int scope)
{	// F-> epsilon
	if(((t->type == SCOPE) && (!scope)) || (t->type == EOF))
		return TRUE;
	else if(t->type == DECLARE)
	{ // F-> DECLARE <Func_line> <F>
		t = get_next(t,LA_S,&storage);
		return (func_line(t,0) && func(t,scope));	
	} 
	else if (t->type == FUNCTION)
	{// F-> <Func_line> <BODY> END Function <F>
		if (func_line(t,1))
			if(body(t))
				if(t->type == END)
				{
					t = get_next(t,LA_S,&storage);
					if(t->type == FUNCTION)
					{
						t = get_next(t,LA_S,&storage);
						if(t->type == EOL)
						{

							/****GENEROVANI MEZIKODU****************/
							codegen_end_function();

							t = get_next(t,LA_S,&storage);
							return func(t,scope);
						}
					}
				}
	}
	else if (t->type == EOL)
	{
		t = get_next(t,LA_S,&storage);
		return func(t,scope);
	}

	return FALSE;
}

int scope(TToken *t)
{  // no scope
	if (t->type == EOF)
		return TRUE;
	else if(t->type == SCOPE)
	{
		t = get_next(t,LA_S,&storage);
		if(t->type == EOL)
		{

			/*****GENEROVANI MEZIKODU****************************************/
			codegen_scope();
			
			free_tree(&root_local);
			t = get_next(t,LA_S,&storage);
			if(body(t))
			{
				if(t->type == END)
				{
					t = get_next(t,LA_S,&storage);
					if(t->type == SCOPE)
					{
						t = get_next(t,LA_S,&storage);
						if((t->type == EOL)||(t->type == EOF)) // oef? 
						{
							t = get_next(t,LA_S,&storage);
							return TRUE;
						}
					}
				}
			}
		}	

	}
	else if (t->type == EOL)
	{
		t = get_next(t,LA_S,&storage);
		return scope(t);
	}
	return FALSE;
}

int parser_start(TToken *t)
{ // S-> FMF
	// insert build in fce
	//TODO 
	semantic_insert_build_in();
	codegen_file_BEGIN();
	t = get_next(t,LA_S,&storage);
	if ((t->type == SCOPE) || (t->type == DECLARE) || (t->type == FUNCTION) ||(t->type == EOF) || (t->type == EOL))
		return (func(t,0) && (scope(t)) && (func(t,1)) && (semantic_call_undefined_fce()));
	return FALSE;	 
}

int parser_FREEBASIC()
{
	TToken* token = NULL;
	token = token_init();

	init_tree(&root_global);
	init_tree(&root_local);


	int res = parser_start(token);

	if(token->type == SCAN_ERR)
		ERROR_RETURN = 1;
	free_tree(&root_local);
	free_tree(&root_global);
	
	// TODO free token
	token_free(token);

	
	return res;
}



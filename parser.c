#include "parser.h"
#include "symbtab.h"
#include "expr.h"
#include "token.h"
#include "semantic.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Ttnode_ptr root_global = NULL;
Ttnode_ptr root_local = NULL;
int return_type = 0;
///////////////////////////
Tstack* LA_S = NULL;
int storage = -2;
///////////////////////////
int ERROR_RETURN = 2;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


int look_ahead(TToken *t, int* type_id, TExpr_operand* operand_array, int* ptr_to_array)
{
	int position = 0;
	char*param = NULL;
	static int call_counter = 0;
	int act_call = call_counter;
	call_counter++;


	TToken new;
	if((new.string = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
		exit(99);
	strcpy(new.string, t->string);

	if((t->type == LENGTH) || (t->type == SUBSTR) || (t->type == ASC) ||(t->type == CHR) )
	{
		int type = t->type; //uchovani pro generovani mezikodu
		
		*type_id = semantic_convert_buildin(t->type);
		if(!semantic_fce_param(root_global,t,&param)){
			
			free(new.string);
			return FALSE;
		}

		t = get_next(t,LA_S,&storage);
		if(t->type == BRACKET_L)
		{
			t = get_next(t,LA_S,&storage);
			if(param_f(t,param,&position)){

				/**GENEROVANI MEZIKODU******************************************/
				int f_return_type;
				switch(type){
					case LENGTH:
						f_return_type = INTEGER;
						codegen_buildin_length();
						break;
					case SUBSTR:
						f_return_type = STRING;
						codegen_buildin_substr();
						break;
					case ASC:
						f_return_type = INTEGER;
						codegen_buildin_asc();
						break;
					case CHR:
						f_return_type = STRING;
						codegen_buildin_chr();
						break;
				}

				printf("DEFVAR LF@&&function_return_%d\n", act_call);
				printf("POPS LF@&&function_return_%d\n", act_call);

				/**pridani vysledku do pole operandu**********************/
				new.type = ID;
				free(new.string);
				if((new.string = malloc(sizeof(char)*(25))) == NULL)
				exit(99);
				sprintf(new.string, "&&function_return_%d", act_call);
				insert_operand_array(&new, operand_array, ptr_to_array, f_return_type);

				if(t->type == BRACKET_R)
				{
					// bacha na off by one
					// TODO AZ Bude semantika od comentovat
					if(position != (int)strlen(param))
					{
						ERROR_RETURN = 4;
						free(new.string);
						return FALSE;
					}
				
				t = get_next(t,LA_S,&storage);
				free(new.string);
				return TRUE;
				}
			}
		}
	}

	new.type = ID;
	t = get_next(t,LA_S,&storage);
	// fce
	if(t->type != BRACKET_L)
	{
	//proměnná 
		Tdata symb;
		Tdata* symbTmp = &symb;
		search_tree(root_local,t->string,symbTmp);
		int return_type = symbTmp->type;
		insert_operand_array(&new, operand_array, ptr_to_array, return_type);
		

		if(!semantic_id_type(root_local,t,type_id)){
			free(new.string);
			return FALSE;
		}
		free(new.string);
		return TRUE;
	}
	else if(t->type == BRACKET_L)
	{
		// semantic chceck
		if(!semantic_fce_param(root_global,t,&param)){
			free(new.string);
			return FALSE;
		}
		if(!semantic_id_type(root_global,t,type_id)){
			free(new.string);
			return FALSE;
		}
		semantic_flag_use(&root_global,t);
		/// end
		t = get_next(t,LA_S,&storage);
		if(param_f(t,param,&position)){


			/**GENEROVANI MEZIKODU*********************************/
			printf("CALL &func&%s\n", new.string);
			printf("DEFVAR LF@&&function_return_%d\n", act_call);
			printf("POPS LF@&&function_return_%d\n", act_call);

			Tdata symb;
			Tdata* symbTmp = &symb;
			search_tree(root_global,new.string,symbTmp);


			int f_return_type = symbTmp->type;	//get function return type

			free(new.string);
			if((new.string = malloc(sizeof(char)*(25))) == NULL)
			exit(99);
			sprintf(new.string, "&&function_return_%d", act_call);

			insert_operand_array(&new, operand_array, ptr_to_array, f_return_type);
			free(new.string);

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
				return TRUE;
			}
		}
	}
	return FALSE;
}


int preprocesing_expr(TToken* t, int condition, int* exp_ret)
{ // TODO - expresion
	char* string;
	if ((string = malloc(sizeof(char)*10)) == NULL)
		exit(99);
	string = strcpy(string,"");

	TExpr_operand *operand_array;
	int ptr_to_array = 0;
	if ((operand_array = malloc(sizeof(struct expr_operand)*10)) == NULL)
		exit(99);
	
	int left_bra = 0;
	int right_bra = 0;
	int type_id = 0;
	int end_loop = 0;

	while(!end_loop)
	{

		if((t->type == EOL) || (t->type == SEMICOLON) || (t->type == THEN) || (t->type == COLON))
			break;
		
		if(strlen(string)%10 == 9)
		{
			int size = strlen(string);
			if ((string = realloc(string, sizeof(char)*(size+10))) == NULL)
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
			case LENGTH:
			case SUBSTR:
			case ASC:
			case CHR:
			case ID: 
					if(!look_ahead(t,&type_id, operand_array, &ptr_to_array))
						{
							free(string);
							operand_array_destructor(operand_array, ptr_to_array);
							return FALSE;	
						}
					string = strcat(string, "i");
					break;
			case INT_V: 
			case FLOAT_V:
			case STRING_V:
					// TODO pro komplikovanejsi strukturu predelat insert
					insert_operand_array(t, operand_array, &ptr_to_array, 0);
					string = strcat(string, "i");
					break;
			case BRACKET_L: left_bra++;
							string = strcat(string, "(");
					break;
			case BRACKET_R:	if(left_bra < ++right_bra)
							{
								end_loop = 1;
								type_id = 1 ; //aby necetlo dalsi token 
								break;
							}
							string = strcat(string, ")");
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
			case LENGTH:
			case SUBSTR:
			case ASC:
			case CHR:
			case ID: if(!look_ahead(t,&type_id, operand_array, &ptr_to_array))
						{
							free(string);
							operand_array_destructor(operand_array, ptr_to_array);
							return FALSE;	
						}
					string = strcat(string, "i");
					break;
			case INT_V:
			case FLOAT_V:
			case STRING_V:
					// TODO pro komplikovanejsi strukturu predelat insert
					insert_operand_array(t, operand_array, &ptr_to_array, 0);
					string = strcat(string, "i");
					break;
			case BRACKET_L: left_bra++;
							string = strcat(string, "(");
					break;
			case BRACKET_R:	if(left_bra < ++right_bra)
							{
								end_loop = 1;
								type_id = 1 ; //aby necetlo dalsi token 
								break;
							}
							string = strcat(string, ")");
					break;
			default:
				free(string);
				operand_array_destructor(operand_array, ptr_to_array);
				return FALSE;	
			
		}
		}	
		if(!type_id)	
			t = get_next(t,LA_S,&storage);
		type_id = 0;
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
	else if (preprocesing_expr(t,0,&nul))	
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


//volani funkce
int param_fn(TToken *t, char* param, int* position)
{
	if(t->type == BRACKET_R){
		return TRUE;
	}
	else if(t->type == COLON)
	{
		t = get_next(t,LA_S,&storage);
		int expr_value;
		if(preprocesing_expr(t,0,&expr_value))
		{
			if(!semantic_check_lside_rside(semantic_convert_data_type(param[(*position)++]),expr_value))
				return FALSE;

			//t = get_next(t,LA_S,&storage);
			return param_fn(t,param,position);
		}
	}
	return FALSE;
}

//volani funkce
int param_f(TToken *t, char* param, int* position)
{
	int expr_value;

	if(t->type == BRACKET_R){
		return TRUE;
	}
	else if(preprocesing_expr(t,0,&expr_value))
	{

		/***dodelat pretypovani*************************************************/
		if(!semantic_check_lside_rside(semantic_convert_data_type(param[(*position)++]),expr_value))
			return FALSE;

		return(param_fn(t,param,position));
	}
	return FALSE;
}

int r_side(TToken *t,int lvalue, char* name)
{
	int rvalue;
	if(preprocesing_expr(t,0,&rvalue))
		{ // <r_side> -> <expr> EOL
			if(t->type == EOL)
			{
				// 
				if(!semantic_check_lside_rside(lvalue,rvalue))
					return FALSE;

				/****GENEROVANI MEZIKODU***************************/
					codegen_assignment(name);
					free(name);


				return TRUE;
			}
		}
	return FALSE;
}


int equal(TToken *t,int lvalue, char* name)
{
	if (t->type == EOL){
		free(name);
		return TRUE;
	}
	else if (t->type == ASSIGN)
	{
		t = get_next(t,LA_S,&storage);
		return r_side(t,lvalue, name);
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
					if(!semantic_insert_id(&root_local,root_global,t->string,&tmp)){
						free(name);
						return FALSE;
					}
					//
					t = get_next(t,LA_S,&storage);

					/****GENEROVANI MEZIKODU********************/
					codegen_dim(name);

					if(equal(t,tmp.type, name))
					{
						if(t->type == EOL)
						{ 
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

		if(!semantic_find_id(t)){
			free(name);
			return FALSE;	
		}
		int lvalue;
		if(!semantic_id_type(root_local,t,&lvalue)){
			free(name);
			return FALSE;
		}

		t = get_next(t,LA_S,&storage);
		if (t->type == ASSIGN)
		{
			t = get_next(t,LA_S,&storage);
			if(r_side(t,lvalue, name))
			{
				if(t->type == EOL)
				{

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
		if(preprocesing_expr(t,0,&nul)){

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
		if(preprocesing_expr(t,1,&nul))
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
			if(preprocesing_expr(t,1,&nul))
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
		if(preprocesing_expr(t,0,&rvalue))
		{
			
			if(return_type == 0)
			{	
				return FALSE;
			}
			if(!semantic_check_lside_rside(return_type,rvalue))
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
	if (t->type == BRACKET_R){
		return TRUE;
	}
	else if(t->type == COLON)
	{
		Tdata tmp;
		t = get_next(t,LA_S,&storage);
		if (t->type == ID)
		{

			char* name = NULL;
			if((name = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
				exit(99);
			strcpy(name, t->string);

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
						if(!semantic_insert_id(&root_local,root_global,t->string,&tmp)){
							free(name);
							return FALSE;	
						}
					}
					t = get_next(t,LA_S,&storage);
					if(params_N(t,data,local)){
						/****GENEROVANI MEZIKODU**********************/
						if(local){
						codegen_func_param(name);
						free(name);
						}
						return TRUE;
					}
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
		char* name = NULL;
		if((name = malloc(sizeof(char)*(strlen(t->string)+1))) == NULL)
			exit(99);
		strcpy(name, t->string);


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
					if(!semantic_insert_id(&root_local,root_global,t->string,&tmp)){
						free(name);
						return FALSE;
					}
				}
				//
				t = get_next(t,LA_S,&storage);
				if(params_N(t,data,local)){

					/****GENEROVANI MEZIKODU***********************/
					if(local){
						//vzdy prvni parametr
						codegen_func_param(name);
						free(name);
					}
					return TRUE;
				}
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
			// semantic
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				char *name =NULL;
				if ((name = malloc(sizeof(char)*strlen(t->string))) == NULL)
					exit(99);
				name = strcpy(name,t->string);

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
						 		if(!semantic_return_type(&return_type,local,tmp.type,name,flag)){
						 			free(name);
						 			return FALSE;
						 		}
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
						 				if(!semantic_check_params(root_global,name,tmp.param)){
						 					free(name);
						 					return FALSE;
						 				}
						 				free(tmp.param);
						 			}
						 			free(name);
						 			t = get_next(t,LA_S,&storage);
						 			return TRUE;
						 		}	
						 	}
						}
					}
					free(name);
			}
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



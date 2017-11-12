#include "parser.h"
#include "symbtab.h"
#include "expr.h"
#include "token.h"
#include "semantic.h"

int semantic_id_type_convert(int type)
{return TRUE;}

int preprocesing_expr(TToken* t, TToken *last, int condition, int* exp_ret)
{ // TODO - expresion
	char* string;
	if ((string = malloc(sizeof(char)*10)) == NULL)
		exit(99);
	string = strcpy(string,"");

	int *type_array;
	int ptr_to_array = 0;
	if ((type_array = malloc(sizeof(int)*10)) == NULL)
		exit(99);
	
	if (last != NULL)
	{
		// TODO pro komplikovanejsi strukturu predelat insert

		type_array[ptr_to_array++] = last->type;
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
			if ((type_array = realloc(type_array, sizeof(int)*(ptr_to_array+10))) == NULL)
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
							free(type_array);
							return FALSE;
						}
			case INT_V: 
			case FLOAT_V:
			case STRING_V:
					// TODO pro komplikovanejsi strukturu predelat insert
					type_array[ptr_to_array++] = semantic_id_type_convert(t->type);
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
				free(type_array);
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
							free(type_array);
							return FALSE;
						}
			case INT_V:
			case FLOAT_V:
			case STRING_V:
					// TODO pro komplikovanejsi strukturu predelat insert
					type_array[ptr_to_array++] = semantic_id_type_convert(t->type);
					string = strcat(string, "i");
					break;
			case BRACKET_L: string = strcat(string, "(");
					break;
			case BRACKET_R: string = strcat(string, ")");
					break;
			default:
				free(string);
				free(type_array);
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

	if(expr(string,condition,postfix) && semantic_exp(postfix,type_array,op_arr,&num_of_op,exp_ret))
		{
		res = TRUE;
		}

	free(string);
	free(type_array);
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
		if(t->type == LENGTH)
		{ // Length (s as string) as integer
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID) ||(t->type == STRING_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s'))
						return FALSE;
					//end
					t = get_next(t,LA_S,&storage);
					if(t->type == BRACKET_R)
					{
						t = get_next(t,LA_S,&storage);
						return TRUE;
					}
				}
			}
		}
		else if(t->type == SUBSTR)
		{ // Substr(s as string, i as integer, n as integer) as STRING
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID)||(t->type == STRING_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s'))
						return FALSE;
					//
					t = get_next(t,LA_S,&storage);
					if(t->type == COLON)
					{
						t = get_next(t,LA_S,&storage);
						if((t->type == ID)||(t->type == INT_V))
						{
							// TODO sementicka kontrola typu
							if(!semantic_id(root_local,t,'i'))
								return FALSE;
							//
							t = get_next(t,LA_S,&storage);
							if(t->type == COLON)
							{
								t = get_next(t,LA_S,&storage);
								if((t->type == ID)||(t->type == INT_V))
								{
									// TODO sementicko kontrolu dat typu
									if(!semantic_id(root_local,t,'i'))
										return FALSE;
									//
									t = get_next(t,LA_S,&storage);
									if(t->type == BRACKET_R)
									{
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
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID) ||(t->type == STRING_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s'))
						return FALSE;
					//end
					t = get_next(t,LA_S,&storage);
					if(t->type == COLON)
					{
						t = get_next(t,LA_S,&storage);
						if((t->type == ID)||(t->type == INT_V))
						{	
							// TODO sementicka kontrola typu
							if(!semantic_id(root_local,t,'i'))
								return FALSE;
							//
							t = get_next(t,LA_S,&storage);
							if(t->type == BRACKET_R)
							{
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
			t = get_next(t,LA_S,&storage);
			if(t->type == BRACKET_L)
			{
				t = get_next(t,LA_S,&storage);
				if((t->type == ID)||(t->type == INT_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'i'))
						return FALSE;
					//
					t = get_next(t,LA_S,&storage);
					if(t->type == BRACKET_R)
					{
						t = get_next(t,LA_S,&storage);
						return TRUE;
					}
				}
			}
		}
	return FALSE;	
}

int param_fn(TToken *t, char* param, int* position)
{
	if(t->type == BRACKET_R)
		return TRUE;
	else if(t->type == COLON)
	{
		t = get_next(t,LA_S,&storage);
		if((t->type == ID )||(t->type == FLOAT_V) ||(t->type == INT_V) ||(t->type == STRING_V))
		{
			if(!semantic_id_param(t,param,position))
				return FALSE;
			t = get_next(t,LA_S,&storage);
			return param_fn(t,param,position);
		}
	}
	return FALSE;
}

int param_f(TToken *t, char* param, int* position)
{
	if(t->type == BRACKET_R)
		return TRUE;
	else if((t->type == ID )||(t->type == FLOAT_V) ||(t->type == INT_V) ||(t->type == STRING_V))
	{
		if(!semantic_id_param(t,param,position))
			return FALSE;
		t = get_next(t,LA_S,&storage);
		return(param_fn(t,param,position));
	}
	return FALSE;
}

int r_side(TToken *t,int lvalue)
{ // zkontrolovat zda se prava strana rovna typove leve
	int rvalue;
	if(t->type == ID)
	{ // TODO zajistit preprocesting id
		TToken tmp = *t;
		t = get_next(t,LA_S,&storage);

		if(t->type == BRACKET_L)
		{ // <r_side> -> id(<param_f>) EOL 
			// TODO kontrolu dat typu
			int position = 0;
			char *param = NULL;
			
			// semantic chceck
			if(!semantic_fce_param(root_global,&tmp,param))
				return FALSE;
			if(!semantic_id_type(root_global,t,&rvalue))
				return FALSE;
			if(!semantic_check_lside_rside(lvalue,rvalue))
				return FALSE;
			/// end
			
			t = get_next(t,LA_S,&storage);
			if(param_f(t,param,&position))
				if(t->type == BRACKET_R)
				{
					// bacha na off by one
					// TODO AZ Bude semantika od comentovat
					/*if(position != strlen(param))
						{
							ERROR_RETURN = 4;
							return FALSE;
						}
					*/
					t = get_next(t,LA_S,&storage);
					if(t->type == EOL)
					{
						return TRUE;
					}
				}
		} 
		else if(preprocesing_expr(t,&tmp,0,&rvalue))
		{ // <r_side> -> <expr> EOL
			if(t->type == EOL)
			{
				// 
				if(!semantic_check_lside_rside(lvalue,rvalue))
					return FALSE;
				return TRUE;
			}
		}
	}	
	else if ((t->type == LENGTH) || (t->type == SUBSTR) || (t->type == ASC) ||(t->type == CHR) )
	{ // BUILD IN FUNCTION
		// TODO SEMANTICKA KONTROLA
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
			if(t->type == EOL)
			{
				// 
				if(!semantic_check_lside_rside(lvalue,rvalue))
					return FALSE;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


int equal(TToken *t,int lvalue)
{
	if (t->type == EOL)
		return TRUE;
	else if (t->type == ASSIGN)
	{
		t = get_next(t,LA_S,&storage);
		return r_side(t,lvalue);
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
					if(!semantic_insert(&root_local,t->string,&tmp))
						return FALSE;
					//
					t = get_next(t,LA_S,&storage);
					if(equal(t,tmp.type))
					{
						if (t->type == EOL)
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
		if(!semantic_find_id(t))
			return FALSE;
		
		int lvalue;
		if(!semantic_id_type(root_local,t,&lvalue))
			return FALSE;

		t = get_next(t,LA_S,&storage);
		if (t->type == ASSIGN)
		{
			t = get_next(t,LA_S,&storage);
			if(r_side(t,lvalue))
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
		if(preprocesing_expr(t,NULL,0,&nul))
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
	else if(t->type == IF)
	{ // <BODY> -> IF <EXP> THEN EOL <BODY> ELSE EOL <BODY> END IF EOL <BODY>
		t = get_next(t,LA_S,&storage);
		if(preprocesing_expr(t,NULL,1,&nul))
			if (t->type == THEN)
			{
				t = get_next(t,LA_S,&storage);
				if(t->type == EOL)
				{
					t = get_next(t,LA_S,&storage);
					if(body(t))
						if(t->type == ELSE)
						{
							t = get_next(t,LA_S,&storage);
							if(t->type == EOL)
							{
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
		t = get_next(t,LA_S,&storage);
		if(t->type == WHILE)
		{
			t = get_next(t,LA_S,&storage);
			if(preprocesing_expr(t,NULL,1,&nul))
				if(t->type == EOL)
				{
					t = get_next(t,LA_S,&storage);
					if(body(t))
						if(t->type == LOOP)
						{
							t = get_next(t,LA_S,&storage);
							if(t->type == EOL)
							{
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
			//semantic
			// ZAPNOUT NA KONTROLU SEMANTIKY TODO
			
			//if(return_type == 0)
			//	return FALSE;
			if(!semantic_check_lside_rside(return_type,rvalue))
				return FALSE;
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

int params_N(TToken *t, Tdata *data, int local)
{
	if (t->type == BRACKET_R)
		return TRUE;
	else if(t->type == COLON)
	{
		Tdata tmp;
		t = get_next(t,LA_S,&storage);
		if (t->type == ID)
		{
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
						if(!semantic_insert(&root_local,t->string,&tmp))
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

int param(TToken* t, Tdata* data, int local)
{ //IMPLICITNE budeme predpokladat ze nebudeme menit string 
	if (t->type == ID)
	{   
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
					if(!semantic_insert(&root_local,t->string,&tmp))
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

	if(t->type == FUNCTION)		
	{ //func_line -> function id (<param>) as <type> eol
		t = get_next(t,LA_S,&storage);
		if(t->type == ID)
		{	
			// sementic
			if(!semantic_insert(&root_global, t->string, &tmp))
				return FALSE;
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
					// nastaveni pro RETURN hodnoty
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
						 		semantic_return_type(return_type,local,tmp.type);
						 		// end  semantic

						 		t = get_next(t,LA_S,&storage);
						 		if(t->type == EOL)
						 		{
						 			// update semantic
						 			insert_data_tree(&root_global, name, &tmp);
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
int func(TToken* t)
{	// F-> epsilon
	if((t->type == SCOPE) || (t->type == EOF))
		return TRUE;
	else if(t->type == DECLARE)
	{ // F-> DECLARE <Func_line> <F>
		t = get_next(t,LA_S,&storage);
		return (func_line(t,0) && func(t));
			
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
							t = get_next(t,LA_S,&storage);
							return func(t);
						}
					}
				}
	}
	else if (t->type == EOL)
	{
		t = get_next(t,LA_S,&storage);
		return func(t);
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

int parser_FREEBASIC(TToken *t)
{ // S-> FMF
	// insert build in fce
	//TODO 
	semantic_insert_build_in();
	t = get_next(t,LA_S,&storage);
	if ((t->type == SCOPE) || (t->type == DECLARE) || (t->type == FUNCTION) ||(t->type == EOF))
		return (func(t) && (scope(t)) && (func(t)) && (semantic_call_undefined_fce()));
	return FALSE;	 
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	TToken* token = NULL;
	token = token_init();

	init_tree(&root_global);
	init_tree(&root_local);


	int res = parser_FREEBASIC(token);
	printf("ERRRRRROR >>>>__%d__<<<<<\n",res);
	free_tree(&root_local);
	free_tree(&root_global);
	
	// TODO free token
	token_free(token);

	
	if(res != TRUE)
		return ERROR_RETURN;
	return 0;
}


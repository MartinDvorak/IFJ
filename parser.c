#include "parser.h"
#include "semantic.h"
#include "symbtab.h"
#include "expr.h"

// GLOBAL VARIABLES FOR symbtab
//*************************************************
Ttnode_ptr root_global = NULL;
Ttnode_ptr root_local = NULL;
int ERROR_RETURN = 2; //syntax error

//***********************************************

TToken* give_me(TToken* t)// simuluje cinost lex.analyzatoru
{
	return t;
}


int preprocesing_expr_real(TToken* t, TToken *last)
{ // TODO - expresion
	char* string;
	if ((string = malloc(sizeof(char)*10)) == NULL)
		exit(99);
	string = strcpy(string,"");

	if (last != NULL)
	{
		// TODO kolize na r_side

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
			case ID: if(!semantic_find_id(t)) 
						return FALSE;
			case INT_V:
			case FLOAT_V:
			case STRING_V:
					string = strcat(string, "i");
					break;
			case BRACKET_L: string = strcat(string, "(");
					break;
			case BRACKET_R: string = strcat(string, ")");
					break;
			default:
				return FALSE;
		}

		t = give_me(t);
	} // indikuje konec vyrazu 
	string = strcat(string, "$");
	return expr(string);

}

int preprocesing_expr(TToken* t,TToken* last)
{ // TODO - expresion
	while(TRUE)
	{
		if((t->type == EOL) || (t->type == SEMICOLON) || (t->type == THEN))
			break;
		
		
		t = give_me(t);
	}
	return TRUE;

}


int expr_n(TToken *t)
{
	if (t->type == EOL)
		return TRUE;
	else if (preprocesing_expr(t,NULL))
	{
		if (t->type == SEMICOLON)
		{
			t = give_me(t);
			return expr_n(t);
		}	
	}
	return FALSE;	
}

int build_in_fce(TToken *t)
{
		if(t->type == LENGTH)
		{ // Length (s as string) as integer
			t = give_me(t);
			if(t->type == BRACKET_L)
			{
				t = give_me(t);
				if((t->type == ID) ||(t->type == STRING_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s'))
						return FALSE;
					//end
					t = give_me(t);
					if(t->type == BRACKET_R)
					{
						t = give_me(t);
						return TRUE;
					}
				}
			}
		}
		else if(t->type == SUBSTR)
		{ // Substr(s as string, i as integer, n as integer) as STRING
			t = give_me(t);
			if(t->type == BRACKET_L)
			{
				t = give_me(t);
				if((t->type == ID)||(t->type == STRING_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s'))
						return FALSE;
					//
					t = give_me(t);
					if(t->type == COLON)
					{
						t = give_me(t);
						if((t->type == ID)||(t->type == INT_V))
						{
							// TODO sementicka kontrola typu
							if(!semantic_id(root_local,t,'i'))
								return FALSE;
							//
							t = give_me(t);
							if(t->type == COLON)
							{
								t = give_me(t);
								if((t->type == ID)||(t->type == INT_V))
								{
									// TODO sementicko kontrolu dat typu
									if(!semantic_id(root_local,t,'i'))
										return FALSE;
									//
									t = give_me(t);
									if(t->type == BRACKET_R)
									{
										t = give_me(t);
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
			t = give_me(t);
			if(t->type == BRACKET_L)
			{
				t = give_me(t);
				if((t->type == ID) ||(t->type == STRING_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'s'))
						return FALSE;
					//end
					t = give_me(t);
					if(t->type == COLON)
					{
						t = give_me(t);
						if((t->type == ID)||(t->type == INT_V))
						{	
							// TODO sementicka kontrola typu
							if(!semantic_id(root_local,t,'i'))
								return FALSE;
							//
							t = give_me(t);
							if(t->type == BRACKET_R)
							{
								t = give_me(t);
								return TRUE;
							}
						}
					}
					
				}
			}
			
		}
		else if(t->type == CHR)
		{ // Chr(i as integer) as string
			t = give_me(t);
			if(t->type == BRACKET_L)
			{
				t = give_me(t);
				if((t->type == ID)||(t->type == INT_V))
				{
					// TODO semantickou kontrolu dat typu
					if(!semantic_id(root_local,t,'i'))
						return FALSE;
					//
					t = give_me(t);
					if(t->type == BRACKET_R)
					{
						t = give_me(t);
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
		t = give_me(t);
		if((t->type == ID )||(t->type == FLOAT_V) ||(t->type == INT_V) ||(t->type == STRING_V))
		{
			if(!semantic_id_param(t,param,position))
				return FALSE;
			t = give_me(t);
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
		t = give_me(t);
		return(param_fn(t,param,position));
	}
	return FALSE;
}

int r_side(TToken *t)
{ // zkontrolovat zda se prava strana rovna typove leve
	if(t->type == ID)
	{ // TODO zajistit preprocesting id
		TToken tmp = *t;
		t = give_me(t);
		if(t->type == BRACKET_L)
		{ // <r_side> -> id(<param_f>) EOL 
			// TODO kontrolu dat typu
			int position = 0;
			char *param = NULL;
			if(!semantic_fce_param(root_global,&tmp,param))
				return FALSE;
			// sementic end
			
			t = give_me(t);
			if(param_f(t,param,&position))
				if(t->type == BRACKET_R)
				{
					// bacha na off by one
					if(position != strlen(param))
						{
							ERROR_RETURN = 4;
							return FALSE;
						}
					
					t = give_me(t);
					if(t->type == EOL)
					{
						t = give_me(t);
						return body(t);
					}
				}
		} 
		else if(preprocesing_expr(t,&tmp))
		{ // <r_side> -> <expr> EOL
			if(t->type == EOL)
			{
				t = give_me(t);
				return body(t);
			}
		}
	}	
	else if ((t->type == LENGTH) || (t->type == SUBSTR) || (t->type == ASC) ||(t->type == CHR) )
	{ // BUILD IN FUNCTION
		if(build_in_fce(t))
		{
			if(t->type == EOL)
			{
				t = give_me(t);
				return body(t);
			}
		}
	}
	
	return FALSE;
}


int equal(TToken *t)
{
	if (t->type == EOL)
		return TRUE;
	else if (t->type == ASSIGN)
	{
		t = give_me(t);
		return r_side(t);
	}
	return FALSE;
}	

int body(TToken *t)
{	// <BODY> -> epsilon
	if( (t->type == END) || (t->type == ELSE) || (t->type == LOOP) )
		return TRUE;
	else if(t->type == DIM)
	{ // <BODY> -> DIM ID AS <TYPE> <=> EOL <BODY>
		t = give_me(t);
		if(t->type == ID)
		{	
			// TODO - insert to local_tree
			Tdata tmp;
			tmp.defined = 1;
			tmp.param = NULL;	
			t = give_me(t);
			if(t->type == AS)
			{
				t = give_me(t);
				if(type(t,&tmp,1,NULL))
				{
					t = give_me(t);
					if(equal(t))
					{
						if (t->type == EOL)
						{ // semantic TODO - predpokladam ze se v token nezmeni t->string
							if(!semantic_insert(&root_local,t->string,&tmp))
								return FALSE;
							// provizorne
							t = give_me(t);
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
		t = give_me(t);
		if (t->type == ASSIGN)
		{
			t = give_me(t);
			if(r_side(t))
			{
				if(t->type == EOL)
				{
					t = give_me(t);
					return body(t);
				}
			}
		}
	}
	else if(t->type == INPUT)
	{ // <BODY> -> INPUT ID EOL <BODY>
		t = give_me(t);
		if(t->type == ID)
		{
			// TODO - semanticka kontrola
			if(!semantic_find_id(t))
				return FALSE;
			//
			t = give_me(t);
			if(t->type == EOL)
			{
				t = give_me(t);
				return body(t);
			}
		}
	}
	else if(t->type == PRINT)
	{ // <BODY> -> PRINt <EXP>; <EXP_N> EOL <BODY>
		t = give_me(t);
		if(preprocesing_expr(t,NULL))
			if(t->type == SEMICOLON)
			{
				t = give_me(t);
				if(expr_n(t))
					if(t->type == EOL)
					{
						t = give_me(t);
						return body(t);
					}
			}
	}
	else if(t->type == IF)
	{ // <BODY> -> IF <EXP> THEN EOL <BODY> ELSE EOL <BODY> END IF EOL <BODY>
		t = give_me(t);
		if(preprocesing_expr(t,NULL))
			if (t->type == THEN)
			{
				t = give_me(t);
				if(t->type == EOL)
				{
					t = give_me(t);
					if(body(t))
						if(t->type == ELSE)
						{
							t = give_me(t);
							if(t->type == EOL)
							{
								t = give_me(t);
								if(body(t))
									if(t->type == END)
									{
										t = give_me(t);
										if(t->type == IF)
										{
											t = give_me(t);
											if(t->type == EOL)
											{
												t = give_me(t);
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
		t = give_me(t);
		if(t->type == WHILE)
		{
			t = give_me(t);
			if(preprocesing_expr(t,NULL))
				if(t->type == EOL)
				{
					t = give_me(t);
					if(body(t))
						if(t->type == LOOP)
						{
							t = give_me(t);
							if(t->type == EOL)
							{
								t = give_me(t);
								return body(t);
							}
						}
				}
		}
	}	
	else if(t->type == RETURN)
	{ // <BODY> -> RETURN <EXP> EOL <BODY>
		// TODO - semanticka kontrolo jestli je return s return typem
		if(preprocesing_expr(t,NULL))
		{
			if(t->type == EOL)
				{
					t = give_me(t);
					return body(t);
				}
		}
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
		case FLOAT: data->param = strcat(data->param,"f");
			break;		
	}
}

int type(TToken* t, Tdata* data, int type, int* to_symbtab)
{
	if ((t->type == INTEGER) || (t->type == STRING) ||(t->type == FLOAT))
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
		t = give_me(t);
		if (t->type == ID)
		{
			t = give_me(t);
			if(t->type == AS)
			{
				t = give_me(t);
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
					t = give_me(t);
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
		t = give_me(t);
		if(t->type == AS)
		{
			t = give_me(t);
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
				t = give_me(t);
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
		t = give_me(t);
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
			t = give_me(t);
			if(t->type == BRACKET_L)
			{
				//semntika -pokud bude definice fce -delam lokalni strom
				if(local)
				{
					free_tree(&root_local);
				}
				//
				t = give_me(t);
				if(param(t, &tmp,local))
					if(t->type == BRACKET_R)
					{
						t = give_me(t);
						if(t->type == AS)
						{
						 	t = give_me(t);
						 	if(type(t,&tmp,1,NULL))
						 	{
						 		t = give_me(t);
						 		if(t->type == EOL)
						 		{
						 			// update semantic
						 			insert_data_tree(&root_global, name, &tmp);
						 			free(name);
						 			t = give_me(t);
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
		t = give_me(t);
		return (func_line(t,0) && func(t));
			
	} 
	else if (t->type == FUNCTION)
	{// F-> <Func_line> <BODY> END Function <F>
		if (func_line(t,1))
			if(body(t))
				if(t->type == END)
				{
					t = give_me(t);
					if(t->type == FUNCTION)
					{
						t = give_me(t);
						if(t->type == EOL)
						{
							t = give_me(t);
							return func(t);
						}
					}
				}
	}

	return FALSE;
}

int scope(TToken *t)
{  // no scope
	if (t->type == EOF)
		return TRUE;
	else if(t->type == SCOPE)
	{
		t = give_me(t);
		if(t->type == EOL)
		{
			t = give_me(t);
			if(body(t))
			{
				if(t->type == END)
				{
					t = give_me(t);
					if(t->type == SCOPE)
					{
						t = give_me(t);
						if(t->type == EOL) // oef? 
						{
							t = give_me(t);
							return TRUE;
						}
					}
				}
			}
		}	

	}
	return FALSE;
}

int parser_FREEBASIC(TToken *t)
{ // S-> FMF
	// insert build in fce
	//TODO 
	semantic_insert_build_in();
	t = give_me(t);
	if ((t->type == SCOPE) || (t->type == DECLARE) || (t->type == FUNCTION) ||(t->type == EOF))
		return (func(t) && (scope(t)) && (func(t)) && (semantic_call_undefined_fce()));
	return FALSE;	 
}

int semantic_call_undefined_fce()
{
	if(search_in_pre_order(root_global))
		{
			ERROR_RETURN = 3;
			return FALSE;
		}

	return TRUE;
}


void semantic_insert_build_in()
{	Tdata tmp;

	tmp.type = INTEGER;
	tmp.defined = 1;
	tmp.param = "s";
	semantic_insert(&root_global,"length",&tmp);

	tmp.type = STRING;
	tmp.defined = 1;
	tmp.param = "sii";
	semantic_insert(&root_global,"substr",&tmp);
	
	tmp.type = INTEGER;
	tmp.defined = 1;
	tmp.param = "si";
	semantic_insert(&root_global,"asc",&tmp);
	
	tmp.type = STRING;
	tmp.defined = 1;
	tmp.param = "i";
	semantic_insert(&root_global,"chr",&tmp);
}


int semantic_convert_data_type (char c)
{
	switch(c)
	{
		case 'i': return INTEGER;
		case 'f': return FLOAT;
		case 's': return STRING; 
		default: return 0;
	}
}

int semantic_id(Ttnode_ptr root, TToken* t, char data_type)
{
	Tdata* tmp = NULL;
	int predict;
	if(t->type == ID)
	{
		if(!search_tree(root,t->string,tmp))
		{
			ERROR_RETURN = 3;
			return FALSE;	
		}
		predict = tmp->type;
	}
	else{
		switch(t->type)
		{
			case INT_V: predict = INTEGER;
				break;
			case FLOAT_V: predict = FLOAT;
				break;
			case STRING_V: predict = STRING;
				break;		
		}
	}	
	int type = semantic_convert_data_type(data_type);
	if(predict == type)
		return TRUE;
	else if ((predict == INTEGER) &&(type == FLOAT))
	{
		// TODO implicit INT -> FLOAT
		return TRUE;
	}
	else if((predict == FLOAT) &&(type == INTEGER))
	{
		// TODO implicit FLOAT -> INT
		return TRUE;	
	}
	else{
		ERROR_RETURN = 4;
		return FALSE;
	}
}

int semantic_id_param(TToken *t, char* param, int* position)
{
	int desizion = t->type;
	
	if (t->type == ID)
	{
 		Tdata* tmp = NULL;
		if(!(search_tree(root_local,t->string,tmp)))
			{
				ERROR_RETURN = 3;
				return FALSE;
			}
		desizion = tmp->type;
	}
	
	// nasel jsem parametr ale mit nema, nebo je vic napsal vic paramentru nez je treba
	if(strlen(param) < (*position)+1)
		{
			ERROR_RETURN = 4;
			return FALSE;
		}
	
	int convert = semantic_convert_data_type((*position)++);


	switch(desizion)
	{
		case INT_V:
		case INTEGER: desizion = INTEGER;
					break;
		case FLOAT_V:
		case FLOAT: desizion = FLOAT;
					break;
		case STRING_V:
		case STRING: desizion = STRING;
					break;
	}

	if(desizion == convert)
	{
		return TRUE;
	}
	else if((desizion == INTEGER) && (convert == FLOAT))
	{
		//TODO implicit convert INT -> FLOAT
		return TRUE; 
	}
	else if((desizion == FLOAT) && (convert == INTEGER))
	{
		//TODO implicit convert FLOAT -> INT
		return TRUE;
	}
	ERROR_RETURN = 4;
	return FALSE;
}


int semantic_fce_param(Ttnode_ptr root, TToken* t, char* param)
{
	Tdata* tmp = NULL;
	if(search_tree(root,t->string,tmp))
	{
		param = tmp->param; 
		return TRUE;
	}
	ERROR_RETURN = 3;
	return FALSE;
}

int semantic_insert(Ttnode_ptr* root, char* name, Tdata* data)
{
	if(insert_tree(root,name,data))
	{
		return TRUE;
	}
	ERROR_RETURN = 3;
	return FALSE;
	// exit code 3    
}

int semantic_find_id(TToken* t)
{
	if(ifdefined(root_local,t->string))
		return TRUE;
	ERROR_RETURN = 3;
	return FALSE;
} 

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	TToken* token = NULL;
	
	init_tree(&root_global);
	init_tree(&root_local);


	int res = parser_FREEBASIC(token);

	free_tree(&root_local);
	free_tree(&root_global);
	
	token_free(token);
	
	if(res != TRUE)
		return ERROR_RETURN;
	return 0;
}
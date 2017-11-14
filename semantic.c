#include "semantic.h"
#include "symbtab.h"

#include "parser.h"


/////////////////////////////////////////////////////////
// 		PRO TESTOVNI SYNTAXE 
////////////////////////////////////////////////////////
/*
void semantic_insert_build_in()
{}

// prevede char na #define hodnotu 'i' -> INTEGER
int semantic_convert_data_type (char c)
{return TRUE;}
// promenou v tokenu vezme najde ve stromu - neni err
// a pote porovna s datovym typev c char data_type
// indikuje implicitni konverze
int semantic_id(Ttnode_ptr root, TToken* t, char data_type)
{return TRUE;}
// stejne jako semantic_id akorat vstup je string param a pohybuje se pomoci position
int semantic_id_param(TToken *t, char* param, int* position)
{return TRUE;}
// nalezne jestli existuje fce a tomom vrátí jeji parametry v  char*param
int semantic_fce_param(Ttnode_ptr root, TToken* t, char* param)
{return TRUE;}

// vlozi pokud neexistuje jinak false
int semantic_insert(Ttnode_ptr* root, char* name, Tdata* data)
{return TRUE;}
// local find id
int semantic_find_id(TToken* t)
{return TRUE;}
// kouka jeslti jsou nejake funkce ktere nejsou definovane ale jsou volane (volane s deklaraci)
int semantic_call_undefined_fce()
{return TRUE;}

int semantic_exp(char* string, int* type_array, Toperation* arr, int* num_of_arr, int* exp_ret)
{return TRUE;}


int semantic_id_type(Ttnode_ptr root,TToken* t,int* type )
{return TRUE;}

int semantic_check_lside_rside(int l_side, int r_side)
{return TRUE;}

void semantic_return_type(int glob_var,int local,int ret_type)
{}
int semantic_id_type_convert(int type)
{return TRUE;}

///////////////////////////////////////////////////////////////////
//				END
///////////////////////////////////////////////////////////////////

*/


void semantic_return_type(int* glob_var,int local,int ret_type)
{
	if(local)
	{
		*glob_var = ret_type;
	}
	else{
		*glob_var = 0;
	}
}

int semantic_check_lside_rside(int l_side, int r_side)
{
	if(l_side == r_side)
	{
		return TRUE;
	}
	else if ((l_side == INTEGER) && (r_side == DOUBLE))
	{	// covreze Double-> int
		return TRUE;
	}
	else if ((l_side == DOUBLE) && (r_side == INTEGER))
	{ // INT -> DOUBLE
		return TRUE;
	}
	
	ERROR_RETURN = 6;
	return FALSE;
	
}

int semantic_id_type_convert(TToken* t)
{
	Tdata tmp;
				
	switch(t->type)
	{
		case INTEGER:
		case INT_V:
				return INTEGER;
		case STRING:
		case STRING_V:
				return STRING;
		case DOUBLE:
		case FLOAT_V:
				return DOUBLE;
		case ID:
				search_tree(root_local,t->string,&tmp);
				return tmp.type;
		default:
			return 0;						

	}
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
	
	if((tmp.param = malloc(sizeof(char)*5)) == NULL)
		exit(99);
	tmp.param = strcpy(tmp.param,"s");
	semantic_insert(&root_global,"length",&tmp);

	tmp.type = STRING;
	tmp.defined = 1;
	
	if((tmp.param = malloc(sizeof(char)*5)) == NULL)
		exit(99);
	tmp.param = strcpy(tmp.param,"sii");
	semantic_insert(&root_global,"substr",&tmp);
	
	tmp.type = INTEGER;
	tmp.defined = 1;
	
	if((tmp.param = malloc(sizeof(char)*5)) == NULL)
		exit(99);	
	tmp.param = strcpy(tmp.param,"si");
	semantic_insert(&root_global,"asc",&tmp);
	
	tmp.type = STRING;
	tmp.defined = 1;

	if((tmp.param = malloc(sizeof(char)*5)) == NULL)
		exit(99);	
	tmp.param = strcpy(tmp.param,"s");
	semantic_insert(&root_global,"chr",&tmp);
}


int semantic_convert_data_type (char c)
{
	switch(c)
	{
		case 'i': return INTEGER;
		case 'f': return DOUBLE;
		case 's': return STRING; 
		default: return 0;
	}
}

int semantic_id(Ttnode_ptr root, TToken* t, char data_type)
{
	Tdata tmp;
	int predict;
	if(t->type == ID)
	{
		if(!search_tree(root,t->string,&tmp))
		{
			ERROR_RETURN = 3;
			return FALSE;	
		}
		predict = tmp.type;
	}
	else{
		switch(t->type)
		{
			case INT_V: predict = INTEGER;
				break;
			case FLOAT_V: predict = DOUBLE;
				break;
			case STRING_V: predict = STRING;
				break;		
		}
	}	
	int type = semantic_convert_data_type(data_type);
	if(predict == type)
		return TRUE;
	else if ((predict == INTEGER) &&(type == DOUBLE))
	{
		// TODO implicit INT -> FLOAT
		return TRUE;
	}
	else if((predict == DOUBLE) &&(type == INTEGER))
	{
		// TODO implicit FLOAT -> INT
		return TRUE;	
	}
	else{
		ERROR_RETURN = 4;
		return FALSE;
	}
}
int semantic_id_type(Ttnode_ptr root,TToken *t, int* type)
{
	Tdata tmp;
	if(!search_tree(root,t->string,&tmp))
	{
		ERROR_RETURN = 3;
		return FALSE;	
	}
	*type = tmp.type;
	return TRUE;
}

int semantic_id_param(TToken *t, char* param, int* position)
{
	int desizion = t->type;
	
	if (t->type == ID)
	{
 		Tdata tmp;
		if(!(search_tree(root_local,t->string,&tmp)))
			{
				ERROR_RETURN = 3;
				return FALSE;
			}
		desizion = tmp.type;
	}
	
	// nasel jsem parametr ale mit nema, nebo je vic napsal vic paramentru nez je treba
	if((int)strlen(param) < (*position)+1)
		{
			ERROR_RETURN = 4;
			return FALSE;
		}
	
	int convert = semantic_convert_data_type(param[(*position)++]);


	switch(desizion)
	{
		case INT_V:
		case INTEGER: desizion = INTEGER;
					break;
		case FLOAT_V:
		case DOUBLE: desizion = DOUBLE;
					break;
		case STRING_V:
		case STRING: desizion = STRING;
					break;
	}

	if(desizion == convert)
	{
		return TRUE;
	}
	else if((desizion == INTEGER) && (convert == DOUBLE))
	{
		//TODO implicit convert INT -> FLOAT
		return TRUE; 
	}
	else if((desizion == DOUBLE) && (convert == INTEGER))
	{
		//TODO implicit convert FLOAT -> INT
		return TRUE;
	}
	ERROR_RETURN = 4;
	return FALSE;
}


int semantic_fce_param(Ttnode_ptr root, TToken* t, char** param)
{
	Tdata tmp;
	if(search_tree(root,t->string,&tmp))
	{
		*param = tmp.param; 
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

int semantic_insert_id(Ttnode_ptr* root,Ttnode_ptr root_check, char* name, Tdata* data)
{
	Tdata tmp;
	if( (insert_tree(root,name,data)) && (search_tree(root_check,name,&tmp)) )
	{
		return TRUE;
	}
	ERROR_RETURN = 3;
	return FALSE;
	// exit code 3    
}

int semantic_check_define(Ttnode_ptr* root, char* name)
{
	Tdata tmp;
	if(search_tree(*root,name,&tmp))
	{
		if (!insert_define_tree(root,name,1,1)){
			return 0;
		}
		return 1;
	}
	return -1;
}

int semantic_check_params(Ttnode_ptr root,char* name, char* param)
{
	Tdata tmp;
	if(search_tree(root,name,&tmp))
	{
		if(strlen(param) != strlen(tmp.param))
		{
			ERROR_RETURN = 3;
			return FALSE;
		}
		else{
			unsigned num = strlen(param);
			for(unsigned i=0 ; i < num ; i++)
				if(param[i] != tmp.param[i])
				{
					ERROR_RETURN = 3;
					return FALSE;
				}
		}
	}
	ERROR_RETURN = 3;
	return FALSE;
}

void semantic_flag_use(Ttnode_ptr* root,TToken* t)
{
	insert_define_tree(root,t->string,1,-1);

}

int semantic_find_id(TToken* t)
{
	if(ifdefined(root_local,t->string))
		return TRUE;
	ERROR_RETURN = 3;
	return FALSE;
} 
	


int semantic_exp(char* string, int* type_array,Toperation* arr, int* num_arr,int* exp_ret)
{
	Tstack* s = NULL;
	s = stack_init();
	int str_num = 0;
	int num_type = 0;
	
	while(string[str_num] != '$')
	{
		if(string[str_num] == 'i')
		{
			// TODO pro komplikovanejsi strukturu predelat insert
			push(s,type_array[num_type++]);
		}
		else{
			
			Toperation tmp;
			
			int left = top2_stack(s);
			int right = top_stack(s);
			char c = string[str_num];
			
			if(((left == STRING)||(right == STRING))&&((c == '-')||(c == '*')||(c == '/')||(c == 'M')))
			{// nepovolena operace nad stringem
				free_stack(s);
				ERROR_RETURN = 4;
				return FALSE;
			}
			else if(left == right)
			{
				if((c == '/')&&(left == INTEGER))
				{
					tmp.op = c;
					tmp.l_convert = 1;
					tmp.r_convert = 1;
					arr[(*num_arr)++] = tmp;
					pop2(s);
					push(s,DOUBLE);

				}
				else if((c == 'M')&&(left = DOUBLE))
				{
					free_stack(s);
					ERROR_RETURN = 4;
					return FALSE;
				}
				else{
					tmp.op = c;
					tmp.l_convert = 0;
					tmp.r_convert = 0;
					arr[(*num_arr)++] = tmp;
					pop(s);
				}
			}
			else if(c == 'M')
			{// modulo nelze jinde nez int/int
				free_stack(s);
				ERROR_RETURN = 4;
				return FALSE;
			}
			else if((left == INTEGER)&&(right == DOUBLE))
			{
				tmp.op = c;
				tmp.l_convert = 1;
				tmp.r_convert = 0;
				arr[(*num_arr)++] = tmp;
				pop2(s);
				push(s,DOUBLE);

			}
			else if((left == DOUBLE)&&(right == INTEGER))
			{
				tmp.op = c;
				tmp.l_convert = 0;
				tmp.r_convert = 1;
				arr[(*num_arr)++] = tmp;
				pop(s);
			}
			else{
				free_stack(s);
				ERROR_RETURN = 4;
				return FALSE;
			}
		}
		str_num++;
	}

	/// tohle pak zachovat nejak zde je implicitni hodnota celeho vyrazu
	*exp_ret = top_stack(s);
	free_stack(s);
	return TRUE;
}

#include "semantic.h"
#include "symbtab.h"
#include "parser.h"

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
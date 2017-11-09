#include "semantic.h"
#include "symbtab.h"
#include "parser.h"

int semantic_call_undefined_fce()
{

	return TRUE;
}


void semantic_insert_build_in()
{

}


int semantic_convert_data_type (char c)
{
	return 0;
}

int semantic_id(Ttnode_ptr root, TToken* t, char data_type)
{
	return true;
}

int semantic_id_param(TToken *t, char* param, int* position)
{
	return true;
}


int semantic_fce_param(Ttnode_ptr root, TToken* t, char* param)
{
	return true;
}

int semantic_insert(Ttnode_ptr* root, char* name, Tdata* data)
{
	return true; 
}

int semantic_find_id(TToken* t)
{
	return true;
} 
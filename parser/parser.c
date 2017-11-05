#include "parser.h"
#include "./symbtab/symbtab.h"

int TEST_SUN=0;
//DOPLNENI TOKENU DO POLE 
int pole[]={SCOPE,EOL,BODY,END,SCOPE,EOL};

#include "token.c"
/*TToken* give_me(TToken* t)// simuluje cinost lex.analyzatoru
{
	return t;
}
*/

int body(TToken *t)
{
	t = give_me(t);
	return TRUE;
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

int type(TToken* t, Tdata* data, int type)
{
	if ((t->type == INTEGER) || (t->type == STRING) ||(t->type == FLOAT))
	{
		if(type)
		{
			data->type = t->type;
		}
		else{
			add_char_to_param(t,data);
		}
		return TRUE;
	}
	return FALSE;
}

int params_N(TToken *t, Tdata *data)
{
	if (t->type == BRACKET_R)
		return TRUE;
	else if(t->type == COLON)
	{
		t = give_me(t);
		if (t->type == ID)
		{
			t = give_me(t);
			if(t->type == AS)
			{
				t = give_me(t);
				if(type(t,data,0))
				{
					t = give_me(t);
					if(params_N(t,data))
						return TRUE;
				}
			}
		}	
	}
	return FALSE;
}

int param(TToken* t, Tdata* data)
{
	if (t->type == ID)
	{
		t = give_me(t);
		if(t->type == AS)
		{
			t = give_me(t);
			if(type(t,data,0))
			{
				t = give_me(t);
				if(params_N(t,data))
					return TRUE;
			}
		}
	}
	else if(t->type == BRACKET_R)
	{
		data->param = NULL;
		return TRUE;
	}
	return FALSE;
}

int func_line(TToken* t,int declared)
{
	Tdata tmp;
	tmp.declared = declared;
	tmp.param = NULL;

	if(t->type == FUNCTION)		
	{ //func_line -> function id (<param>) as <type> eol
		t = give_me(t);
		if(t->type == ID)
		{	
			// ADD ID TODO to some tmp
			t = give_me(t);
			if(t->type == BRACKET_L)
			{
				t = give_me(t);
				if(param(t, &tmp))
					if(t->type == BRACKET_R)
					{
						t = give_me(t);
						if(t->type == AS)
						{
						 	t = give_me(t);
						 	if(type(t,&tmp,1))
						 	{
						 		t = give_me(t);
						 		if(t->type == EOL)
						 		{
						 			t = give_me(t);
						 			printf ("deklarafe funkce korektni\n");
						 			return TRUE;
						 		}	
						 	}
						}
					}
			}
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
		return (func_line(t,1) && func(t));
			
	} 
	else if (t->type == FUNCTION)
	{// F-> <Func_line> <BODY> END Function <F>
		if (func_line(t,0))
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
							printf ("Definice probehla uspesne\n");
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
							printf ("Korektni kostra programu\n");
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
	t = give_me(t);
	if ((t->type == SCOPE) || (t->type == DECLARE) || (t->type == FUNCTION) ||(t->type == EOF))
		return (func(t) && (scope(t)) && (func(t)));
	return FALSE;	 
}

int main(int argc, char ** argv)
{
	(void)argc;
	(void)argv;
	TToken* token = NULL;

	parser_FREEBASIC(token);

	return 0;
}
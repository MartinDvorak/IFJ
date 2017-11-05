#include "token.h"

TToken* token_init()
{
	TToken* t;

	if ((t = malloc(sizeof(TToken))) == NULL)
		exit(-1);
	t->int_v = 0;
	t->float_v = 0.0;
	t->string = NULL;
	return t;
}

void token_free(TToken* t)
{
	if (t->string != NULL)
		free(t->string);
	free(t);
}


TToken* give_me(TToken* t)// simuluje cinost lex.analyzatoru
{
	if (t == NULL) //FIRST USE
	{
		t = token_init();	
	}
	
	free(t->string);
	if ((t->string = malloc(sizeof(char)*20)) == NULL)
		{exit(-1);}
	t->string= strcpy(t->string,"\0");
	t->type = pole[TEST_SUN];
	TEST_SUN++;
	// INSERT DATA
	return t;
}



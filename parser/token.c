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

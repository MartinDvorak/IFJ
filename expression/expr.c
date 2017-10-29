
#include "expr.h"



void stack_realloc(Tstack* s)
{
	s->max += 10;
	if((s->bottom = realloc(s->bottom, sizeof(char)*s->max)) == NULL)
		exit(-1); // TODO interni chyba

}

Tstack* stack_init()
{
	Tstack* s;
	if ((s = malloc(sizeof(Tstack))) == NULL)
		exit(-1); // TODO interni chyba interpretu 
	s->top = 0;
	s->max = 20;
	if((s->bottom = malloc(sizeof(char)*s->max)) == NULL)
		exit(-1); // TODO interni chyba prekladace
	return s;
}

char pop(Tstack* s)
{
	return(s->bottom[s->top--]);
}

void printstack(Tstack* s)
{
	printf("BOTTOM>>");
	for (int i=0; i <= s->top; i++)
		printf("%c", s->bottom[i]);
	printf("\n");

}

void push(Tstack* s, char insert)
{
	s->top++;
	if(s->top == s->max)
	{
		stack_realloc(s);
	}
	s->bottom[s->top] = insert;
}

void stack_push_lt(Tstack* s)
{
	char c = top_stack_terminal(s);
	int i;
	
	for (i=s->top; i >= 0 ; i-- )
		if (c == s->bottom[i])
			break;

	int size = s->top - i; 
	if (size)
	{
		char tmp[size];

		for (int j=0; j < size ; j++)
		{
			tmp[j] = pop(s);
		}
		push(s,'<');
		for (int j=size-1; j >= 0 ; j--)
		{
			push(s,tmp[j]);
		}
	}
	else{
		push(s,'<');
	}
}

char top_stack_terminal(Tstack* s)
{
	for(int i=s->top; i >= 0; i--)
		if((s->bottom[i] != 'E')&&(s->bottom[i] != '<')) 
			return s->bottom[i];
}


void free_stack(Tstack* s)
{
	free(s->bottom);
	free(s);
}

int reduce(Tstack*s)
{
	// E -> i
	if ((s->bottom[s->top] == 'i')&&(s->bottom[s->top-1] == '<'))
	{
		s->top--;
		s->bottom[s->top] = 'E';
	}	// E -> E * E 
	else if(((s->bottom[s->top] == 'E')&&(s->bottom[s->top-1] == '*'))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E M E 
	else if(((s->bottom[s->top] == 'E')&&(s->bottom[s->top-1] == 'M'))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E + E 
	else if(((s->bottom[s->top] == 'E')&&(s->bottom[s->top-1] == '+'))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E L E 
	else if(((s->bottom[s->top] == 'E')&&(s->bottom[s->top-1] == 'L'))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> ( E ) 
	else if(((s->bottom[s->top] == ')')&&(s->bottom[s->top-1] == 'E'))&&((s->bottom[s->top-2] == '(')&&(s->bottom[s->top-3] == '<')))
	{
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}
	else{
		return FALSE;
	}
	return TRUE;

}

char table_precedence(char** table,char token_symbol, char stack_symbol)
{
	int i,j;
	
	for (i=0; i < 9; i++)
		if(table[0][i] == token_symbol)
			break;

	for (j=0; j < 9; j++)
		if(table[j][0] == stack_symbol)
			break;

	return table[j][i];
}

char **init_table_precedence()
{
	char **table;
	
	if ((table = malloc(sizeof(char*)*9)) == NULL)
		exit(-1);

	for (int i=0; i < 9 ; i++)
	{
		if((table[i] = malloc(sizeof(char)*10)) == NULL)
		exit(-1);
	}

	table[0] = strcpy(table[0], "0*M+L()i$\0");	
	table[1] = strcpy(table[1], "*>>>><><>\0");
	table[2] = strcpy(table[2], "M<>>><><>\0");
	table[3] = strcpy(table[3], "+<<>><><>\0");
	table[4] = strcpy(table[4], "L<<<0<><>\0");
	table[5] = strcpy(table[5], "(<<<<<=<0\0");
	table[6] = strcpy(table[6], ")>>>>0>0>\0");
	table[7] = strcpy(table[7], "i>>>>0>0>\0");
	table[8] = strcpy(table[8], "$<<<<<0<0\0");	

	return table;
}


void free_table(char** table)
{
	for (int i=0; i < 9; i++)
	{
		free(table[i]);
	}	
	free(table);
}
#include "stack.h"


void stack_realloc(Tstack* s)
{
	s->max += 10;
	if((s->bottom = realloc(s->bottom, sizeof(char)*s->max)) == NULL)
		exit(99); // TODO interni chyba

}

Tstack* stack_init()
{
	Tstack* s;
	if ((s = malloc(sizeof(Tstack))) == NULL)
		exit(99); // TODO interni chyba interpretu 
	s->top = -1;
	s->max = 20;
	if((s->bottom = malloc(sizeof(char)*s->max)) == NULL)
		exit(99); // TODO interni chyba prekladace
	return s;
}

int stack_empty(Tstack* s)
{
	return (s->top == -1);
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

char top_stack(Tstack* s)
{
	if (s->top != -1)
		return s->bottom[s->top];
}

char read_top_pop(Tstack* s)
{
	if(s->top != -1)
		return s->bottom[s->top--];
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

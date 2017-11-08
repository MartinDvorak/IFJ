
#include "expr.h"
#include "stack.h"

char next(char* string, int *position){
	return string[(*position)++];
}

int expr(char* string){

	int position = 0;
	Tstack* s = NULL;

	s = stack_init();
	push(s,'$');
	
	char input;
	char top;
	char action;

	char **table = init_table_precedence();
	input = next(string,&position);
	top = top_stack_terminal(s);	
	do{

		action = table_precedence(table,input,top);
		switch(action)
		{
			case '=':push(s,input);
					input = next(string,&position);	
					break;
			case '<':stack_push_lt(s);
					push(s,input);
					input = next(string,&position);
					break;
			case '>':if(reduce(s))
						break;
					else{return FALSE;}
			default:free_stack(s);
					free_table(table);
					return FALSE;
		}
		top = top_stack_terminal(s);

	}while(!((input == '$')&&(top == '$')));

	free_table(table);
	free_stack(s);
	free(string);
	return TRUE;
}

int reduce(Tstack*s)
{
	// E -> i
	if ((s->bottom[s->top] == 'i')&&(s->bottom[s->top-1] == '<'))
	{
		printf("%c", s->bottom[s->top]);
		s->top--;
		s->bottom[s->top] = 'E';
	}	// E -> E * E 
	else if(((s->bottom[s->top] == 'E')&&((s->bottom[s->top-1] == '*')||(s->bottom[s->top-1] == '/')))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
	 	printf("%c", s->bottom[s->top-1]);
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E M E 
	else if(((s->bottom[s->top] == 'E')&&(s->bottom[s->top-1] == 'M'))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		printf("%c", s->bottom[s->top-1]);
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E + E 
	else if(((s->bottom[s->top] == 'E')&&((s->bottom[s->top-1] == '+')||(s->bottom[s->top-1] == '-')))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		printf("%c", s->bottom[s->top-1]);
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E L E 
	else if(((s->bottom[s->top] == 'E')&&( (s->bottom[s->top-1] == '=') || (s->bottom[s->top-1] == 'N') || (s->bottom[s->top-1] == 'L') || (s->bottom[s->top-1] == 'G') || (s->bottom[s->top-1] == 'S') || (s->bottom[s->top-1] == 'R')))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		printf("%c", s->bottom[s->top-1]);
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
	
	for (i=0; i < 16; i++)
		if(table[0][i] == token_symbol)
			break;

	for (j=0; j < 16; j++)
		if(table[j][0] == stack_symbol)
			break;

	return table[j][i];
}

char **init_table_precedence()
{// TODO mozna budoou dve verze podle toho pro jaky neterminal
	char **table;
	
	if ((table = malloc(sizeof(char*)*16)) == NULL)
		exit(-1);

	for (int i=0; i < 16 ; i++)
	{
		if((table[i] = malloc(sizeof(char)*17)) == NULL)
		exit(-1);
	}
/*
	table[0] = strcpy(table[0], "0*M+L()i$\0");	
	table[1] = strcpy(table[1], "*>>>><><>\0");
	table[2] = strcpy(table[2], "M<>>><><>\0");
	table[3] = strcpy(table[3], "+<<>><><>\0");
	table[4] = strcpy(table[4], "L<<<0<><>\0");
	table[5] = strcpy(table[5], "(<<<<<=<0\0");
	table[6] = strcpy(table[6], ")>>>>0>0>\0");
	table[7] = strcpy(table[7], "i>>>>0>0>\0");
	table[8] = strcpy(table[8], "$<<<<<0<0\0");	

*/

	table[0] = strcpy(table[0], "0*/M+-=NLGSR()i$\0");	
	table[1] = strcpy(table[1], "*>>>>>>>>>>><><>\0");
	table[2] = strcpy(table[2], "/>>>>>>>>>>><><>\0");
	table[3] = strcpy(table[3], "M<<>>>>>>>>><><>\0");
	table[4] = strcpy(table[4], "+<<<>>>>>>>><><>\0");
	table[5] = strcpy(table[5], "-<<<>>>>>>>><><>\0");
	table[6] = strcpy(table[6], "=<<<<<>>>>>><><>\0");
	table[7] = strcpy(table[7], "N<<<<<>>>>>><><>\0");
	table[8] = strcpy(table[8], "L<<<<<>>>>>><><>\0");
	table[9] = strcpy(table[9], "G<<<<<>>>>>><><>\0");
  table[10] = strcpy(table[10], "S<<<<<>>>>>><><>\0");
  table[11] = strcpy(table[11], "R<<<<<>>>>>><><>\0");
  table[12] = strcpy(table[12], "(<<<<<<<<<<<<=<0\0");
  table[13] = strcpy(table[13], ")>>>>>>>>>>>0>0>\0");
  table[14] = strcpy(table[14], "i>>>>>>>>>>>0>0>\0");
  table[15] = strcpy(table[15], "$<<<<<<<<<<<<0<0\0");	

	return table;
}


void free_table(char** table)
{
	for (int i=0; i < 16; i++)
	{
		free(table[i]);
	}	
	free(table);
}


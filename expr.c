
#include "expr.h"
#include "stack.h"

char next(char* string, int *position){
	return string[(*position)++];
}

int expr(char* string,int condition, char* postfix){

	int position = 0;
	Tstack* s = NULL;

	s = stack_init();
	push(s,'$');
	
	char input;
	char top;
	char action;
	int position_postfix = 0;

	char **table = init_table_precedence(condition);
	input = next(string,&position);
	top = top_stack_terminal(s);	
	do{

		action = table_precedence(table,input,top,condition);
		switch(action)
		{
			case '=':push(s,input);
					input = next(string,&position);	
					break;
			case '<':stack_push_lt(s);
					push(s,input);
					input = next(string,&position);
					break;
			case '>':if(reduce(s,postfix,&position_postfix))
						break;
					else{					
						free_table(table,condition);
						free_stack(s);
						return FALSE;
					}
			default:free_stack(s);
					free_table(table,condition);
					free_stack(s);
					return FALSE;
		}
		top = top_stack_terminal(s);

	}while(!((input == '$')&&(top == '$')));

	postfix[position_postfix] = '$';

	free_table(table,condition);
	free_stack(s);
	return TRUE;
}

int reduce(Tstack*s, char* postfix, int* position_postfix)
{
	// E -> i
	if ((s->bottom[s->top] == 'i')&&(s->bottom[s->top-1] == '<'))
	{
		postfix[(*position_postfix)++] = s->bottom[s->top]; 
		//printf("%c", s->bottom[s->top]);
		s->top--;
		s->bottom[s->top] = 'E';
	}	// E -> E * E 
	else if(((s->bottom[s->top] == 'E')&&((s->bottom[s->top-1] == '*')||(s->bottom[s->top-1] == '/')))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		postfix[(*position_postfix)++] = s->bottom[s->top-1]; 
	 	//printf("%c", s->bottom[s->top-1]);
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E M E 
	else if(((s->bottom[s->top] == 'E')&&(s->bottom[s->top-1] == 'M'))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		postfix[(*position_postfix)++] = s->bottom[s->top-1];
		//printf("%c", s->bottom[s->top-1]);
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E + E 
	else if(((s->bottom[s->top] == 'E')&&((s->bottom[s->top-1] == '+')||(s->bottom[s->top-1] == '-')))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		postfix[(*position_postfix)++] = s->bottom[s->top-1];
		//printf("%c", s->bottom[s->top-1]);
		s->top -= 3;
		s->bottom[s->top] = 'E';
	}	// E -> E L E 
	else if(((s->bottom[s->top] == 'E')&&( (s->bottom[s->top-1] == '=') || (s->bottom[s->top-1] == 'N') || (s->bottom[s->top-1] == 'L') || (s->bottom[s->top-1] == 'G') || (s->bottom[s->top-1] == 'S') || (s->bottom[s->top-1] == 'R')))&&((s->bottom[s->top-2] == 'E')&&(s->bottom[s->top-3] == '<')))
	{
		postfix[(*position_postfix)++] = s->bottom[s->top-1];
		//printf("%c", s->bottom[s->top-1]);
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

char table_precedence(char** table,char token_symbol, char stack_symbol,int condition)
{
	int size_col = 10;
	if(condition)
	{
		size_col = 16;
	}

	int i,j;
	
	for (i=0; i < size_col; i++)
		if(table[0][i] == token_symbol)
			break;

	for (j=0; j < size_col; j++)
		if(table[j][0] == stack_symbol)
			break;

	return table[j][i];
}

char **init_table_precedence(int condition)
{// TODO mozna budoou dve verze podle toho pro jaky neterminal
	char **table;
	int size_col = 10;
	int size_row = 11;
	if(condition)
	{
		size_col = 16;
		size_row = 17;
	}

	if ((table = malloc(sizeof(char*)*size_col)) == NULL)
		exit(-1);

	for (int i=0; i < size_col ; i++)
	{
		if((table[i] = malloc(sizeof(char)*size_row)) == NULL)
		exit(-1);
	}
	
	if(condition){
	table[0] = strcpy(table[0], "0*/M+-=NLGSR()i$\0");	
	table[1] = strcpy(table[1], "*>>>>>>>>>>><><>\0");
	table[2] = strcpy(table[2], "/>>>>>>>>>>><><>\0");
	table[3] = strcpy(table[3], "M<<>>>>>>>>><><>\0");
	table[4] = strcpy(table[4], "+<<<>>>>>>>><><>\0");
	table[5] = strcpy(table[5], "-<<<>>>>>>>><><>\0");
	table[6] = strcpy(table[6], "=<<<<<000000<><>\0");
	table[7] = strcpy(table[7], "N<<<<<000000<><>\0");
	table[8] = strcpy(table[8], "L<<<<<000000<><>\0");
	table[9] = strcpy(table[9], "G<<<<<000000<><>\0");
  table[10] = strcpy(table[10], "S<<<<<000000<><>\0");
  table[11] = strcpy(table[11], "R<<<<<000000<><>\0");
  table[12] = strcpy(table[12], "(<<<<<<<<<<<<=<0\0");
  table[13] = strcpy(table[13], ")>>>>>>>>>>>0>0>\0");
  table[14] = strcpy(table[14], "i>>>>>>>>>>>0>0>\0");
  table[15] = strcpy(table[15], "$<<<<<<<<<<<<0<0\0");	
	}

	else
	{	
	table[0] = strcpy(table[0], "0*/M+-()i$\0");	
	table[1] = strcpy(table[1], "*>>>>><><>\0");
	table[2] = strcpy(table[2], "/>>>>><><>\0");
	table[3] = strcpy(table[3], "M<<>>><><>\0");
	table[4] = strcpy(table[4], "+<<<>><><>\0");
	table[5] = strcpy(table[5], "-<<<>><><>\0");
	table[6] = strcpy(table[6], "(<<<<<<=<0\0");
	table[7] = strcpy(table[7], ")>>>>>0>0>\0");
	table[8] = strcpy(table[8], "i>>>>>0>0>\0");
	table[9] = strcpy(table[9], "$<<<<<<0<0\0");	
	}
	
	return table;
}


void free_table(char** table,int condition)
{
	int size_col = 10;
	if(condition)
	{
		size_col = 16;
	}
	for (int i=0; i < size_col; i++)
	{
		free(table[i]);
	}	
	free(table);
}


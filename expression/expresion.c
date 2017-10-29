
#include "expr.h"




/********************
Komentář 
i = identifikátor, constanta ( jedno jakého typu je mi to jedno= neřeší syntexe)

* = reprezentu je třídu operácí {*, /}
M = reprezentu je třídu operácí {\}
+ = reprezentu je třídu operácí {+, -}
L = reprezentu je třídu operácí {<, > , <>, <=, >=, == }

(,) zavorky výrazů

$ = ukončovací retezec vstupu


v ukazateli string viz níže (24.řádek) se dosadí testovací podmínka podle předchozích pravidel.
*/
int position = 0;
char *string;

char give_me_bitch(){
	return string[position++];
	// TODO PREDELAT NA TOKEN
}

int expr(){

	Tstack* s;

	s = stack_init();
	push(s,'$');
	
	char input;
	char top;
	char action;

	char **table = init_table_precedence();
	input = give_me_bitch();
	top = top_stack_terminal(s);	
	do{
		action = table_precedence(table,input,top);
		switch(action)
		{
			case '=':push(s,input);
					input = give_me_bitch();	
					break;
			case '<':stack_push_lt(s);
					push(s,input);
					input = give_me_bitch();
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
	return TRUE;
}

int main(int argc, char **argv)
{
	string = malloc(sizeof(char)*20);
	string = strcpy(string, argv[1]);

	printf("%s\n", string);

	printf("%d\n", expr());
	free(string);
	return 0;
}
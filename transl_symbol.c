#define VAR 	1
#define INTEGER 2
#define DOUBLE  3
#define STRING  4

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct symbol{
	
	int type;
	char* var_name;
	char operand;
	int ivalue;
	double dvalue;
	char* svalue;

}tSymbol;


/*
operandy:

	int type = { '+' ,'-' , '*' , '/' , 'M' , '<'  , '>' , 'L' <= , 'G' >= , 'E' = , 'N' <> }

konstanta:

	 int type = { INTEGER , DOUBLE , STRING }
	 hodnota v prislusne promenne struktury

promenna:

	int type = VAR
	char* var_name = jmeno;
*/

void symb_arr_destructor(tSymbol* arr, int size){

	//printf("%c\n", arr[0].type);

	
	for(int i = 0; i < size ;i++){

		if(arr[i].type = STRING){
			//konstanta retezec
			free(arr[i].svalue);
		}
		if(arr[i].type = VAR){
			//promenna
			free(arr[i].var_name);
		}	
	}

	free(arr);
}




////example of alloc
int main(){


	tSymbol* parray;
	parray = (tSymbol*) malloc(10*sizeof(struct symbol));
	
	symb_arr_destructor(parray, 10);
}


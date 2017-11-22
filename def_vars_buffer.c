#include "def_vars_buffer.h"



//inicializuje pole pro definonave promenne
void init_def_vars_buffer(tVarsBuffer** defined_vars){

	if(((*defined_vars) = malloc(sizeof(struct vars_buffer))) == NULL)
		exit(99);

	if(((*defined_vars)->arr = (char**) malloc(sizeof(char*)*10)) == NULL)
		exit(99);

	for(int i = 0; i < 1; i++){
		(*defined_vars)->arr[i] = NULL;
	}

	(*defined_vars)->size = 10;
	(*defined_vars)->used = 0;
}


//uvolni pamet bufferu 
void free_def_vars_buffer(tVarsBuffer** defined_vars){

	for(int i = 0; i < (*defined_vars)->used; i++){
			free((*defined_vars)->arr[i]);
		}

		free((*defined_vars)->arr);
		free((*defined_vars));
}


//pouze odstrani vsechny promenne, nezrusi buffer
void clean_def_vars_buffer(tVarsBuffer** defined_vars){

	for(int i = 0; i < (*defined_vars)->used; i++){
		free((*defined_vars)->arr[i]);
	}	

	(*defined_vars)->used = 0;
}


//prida pormennou do pole
void add_defined_variable(tVarsBuffer** defined_vars, char* name){

	if((*defined_vars)->used == (*defined_vars)->size){
		//zvetseni bufferu 
		if(((*defined_vars)->arr = realloc((*defined_vars)->arr, sizeof(char)*((*defined_vars)->size + 10))) == NULL)
			exit(99);
	} 

	if(((*defined_vars)->arr[(*defined_vars)->used] = malloc(sizeof(char)*(strlen(name)+1))) == NULL)
		exit(99);

	strcpy((*defined_vars)->arr[(*defined_vars)->used], name);

	(*defined_vars)->used++;
}


//hleda promennou v poli definovanych, TRUE pokud nalezne, FALSE pokud ne
int lookup_var_in_buffer(tVarsBuffer** defined_vars, char* name){

	for(int i = 0; i < (*defined_vars)->used; i++){

		if(!strcmp(name, (*defined_vars)->arr[i])){
			return TRUE;
		}
	}

	return FALSE;
}

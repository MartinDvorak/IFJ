#include "codegen.h"

#ifndef VAR_BUFFER
typedef struct vars_buffer{

	char** arr;
	int size;
	int used;

}tVarsBuffer;
#endif
#define VAR_BUFFER

void init_def_vars_buffer(tVarsBuffer** defined_vars);

void free_def_vars_buffer(tVarsBuffer** defined_vars);

void clean_def_vars_buffer(tVarsBuffer** defined_vars);

void add_defined_variable(tVarsBuffer** defined_vars, char* name);

int lookup_var_in_buffer(tVarsBuffer** defined_vars, char* name);
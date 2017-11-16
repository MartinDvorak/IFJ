
#include "parser.h"


int main(int argc, char ** argv)
{
	(void)argc;
	(void)argv;

	int res = parser_FREEBASIC();

	if (!res)
	{
		printf("\nVystup z prekladace::%d::\n", ERROR_RETURN);
	}
	else{
		printf("\nVystup z prekladace::%d::\n", res);	
	} 
	
	if(res != TRUE)
		return ERROR_RETURN;
	return 0; 
}
#include "parser.h"


int main(int argc, char ** argv)
{
	(void)argc;
	(void)argv;

	int res = parser_FREEBASIC();

/*
	if (!res)
	{
		printf("Vystup z prekladace::%d::\n", ERROR_RETURN);
	}
	else{
		printf("Vystup z prekladace::%d::\n", res);	
	} 
	
*/
	if(res != TRUE)
		return ERROR_RETURN;
	return 0;
}
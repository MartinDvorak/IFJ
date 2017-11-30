#include "parser.h"


int main(int argc, char ** argv)
{
	(void)argc;
	(void)argv;

	int res = parser_FREEBASIC();

	if(res != TRUE)
		return ERROR_RETURN;
	return 0;
}

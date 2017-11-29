#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char ** argv)
{
	int a = 42;
	int b = 0;
	char *neco = NULL;
	
	neco = malloc(sizeof(char)*10);
	neco = strcpy(neco,"");
	
	printf("%d\n", strlen(neco));

	if (!(a && b))
	{
		printf("HELL\n");
	}
	return 0;
}
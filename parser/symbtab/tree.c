


#include "symbtab.h"

void Print_tree2(Ttnode_ptr TempTree, char* sufix, char fromdir)
/* vykresli sktrukturu binarniho stromu */

{
     if (TempTree != NULL)
     {
	char* suf2 = (char*) malloc(strlen(sufix) + 4);
	strcpy(suf2, sufix);
        if (fromdir == 'L')
	{
	   suf2 = strcat(suf2, "  |");
	   printf("%s\n", suf2);
	}
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->rptr, suf2, 'R');
        printf("%s  +-[%c,%d]\n", sufix, TempTree->key[0], TempTree->data.type);
	strcpy(suf2, sufix);
        if (fromdir == 'R')
	   suf2 = strcat(suf2, "  |");	
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->lptr, suf2, 'L');
	if (fromdir == 'R') printf("%s\n", suf2);
	free(suf2);
    }
}

void Print_tree(Ttnode_ptr TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
} 

int main(int argc, char ** argv)
{
	Ttnode_ptr root = NULL;

	init_tree(&root);

	Tdata tmp;
	tmp.param = NULL;
	tmp.type = 10;
	tmp.defined = TRUE;

	printf("%d\n",insert_tree(&root, "fce", &tmp));
	printf("%d\n",insert_tree(&root, "dce", &tmp));
	printf("%d\n",insert_tree(&root, "ace", &tmp));
	printf("%d\n",insert_tree(&root, "hce", &tmp));
	printf("%d\n",insert_tree(&root, "wce", &tmp));
	printf("%d\n",insert_tree(&root, "rce", &tmp));
	tmp.param = malloc(sizeof(char)*10);
	tmp.param = strcpy(tmp.param, "iif");
	

	printf("%d\n",insert_tree(&root, "bce", &tmp));

	Tdata data;
	printf("SEARCH >>%d\n", search_tree(root, "bce", &data));
	printf(">>%i><\n",data.defined);
	printf("node >bce< and params are :  %s\n", data.param);
	printf("IF_defined >%d<\n", ifdefined(root, "bce"));

	Print_tree(root);
	free_tree(&root);
	return 0;





























}
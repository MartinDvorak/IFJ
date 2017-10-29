


#include "symbtab.h"

void Print_tree2(Ttnode_fce_ptr TempTree, char* sufix, char fromdir)
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
        printf("%s  +-[%c,%d]\n", sufix, TempTree->key[0], TempTree->data.state);
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

void Print_tree(Ttnode_fce_ptr TempTree)
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
	Ttnode_fce_ptr root = NULL;

	init_symtab_fce(&root);

	Tdata_fce tmp;
	tmp.param_list = NULL;
	tmp.type_return = 10;
	tmp.state = 42;

	printf("%d\n",insert_symtab_fce(&root, "fce", &tmp));
	printf("%d\n",insert_symtab_fce(&root, "dce", &tmp));
	printf("%d\n",insert_symtab_fce(&root, "ace", &tmp));
	printf("%d\n",insert_symtab_fce(&root, "hce", &tmp));
	printf("%d\n",insert_symtab_fce(&root, "wce", &tmp));
	printf("%d\n",insert_symtab_fce(&root, "rce", &tmp));
	tmp.param_list = malloc(sizeof(char)*10);
	tmp.param_list = strcpy(tmp.param_list, "neco");
	

	printf("%d\n",insert_symtab_fce(&root, "bce", &tmp));

	Tdata_fce data;
	printf("SEARCH >>%d\n", search_symtab_fce(root, "bce", &data));
	
	printf(">>%s><\n",data.param_list);
	
	Ttnode_id_ptr root_id = NULL;
	
	printf("%d\n", get_symtab_fces_variables(root, "rce", &root_id));


	Tdata_id ttmp;
	ttmp.type = 7;
	ttmp.state = 20;

	init_symtab_id(&)

	Print_tree(root);
	free_symtab_fce_all(&root);
	return 0;
}
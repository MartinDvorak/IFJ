
#include "symbtab.h"

// Function tree
void init_symtab_fce(Ttnode_fce_ptr* root){
	*root = NULL;
}

int insert_symtab_fce(Ttnode_fce_ptr* root, char* name, Tdata_fce* data){
	if (*root == NULL)
	{
		if ((*root = malloc(sizeof(struct tnode_fce))) == NULL)
			exit(-1);// magical constant 7 for save
		if (((*root)->key = malloc(sizeof(char)*(strlen(name)+7))) == NULL) 
			exit(-1);
		(*root)->key = strcpy((*root)->key ,name);
		(*root)->data = *data;
		(*root)->variables = NULL;
		(*root)->lptr = NULL;
		(*root)->rptr = NULL;
		return TRUE;
	}
	else{
		if (strcmp(((*root)->key),name) > 0)
			return insert_symtab_fce(&(*root)->lptr, name, data);
		else if (strcmp(((*root)->key),name) < 0)
			return insert_symtab_fce(&(*root)->rptr, name, data);
		else{
			return FALSE;
		}
	}
}

// TRUE or FALSE if true data returned in Tdata_fce* data
int  search_symtab_fce(Ttnode_fce_ptr root ,char* name, Tdata_fce* tdata){
	if (root == NULL)
	{
		return FALSE;
	}
	else{
		if (!(strcmp(root->key, name)))
		{
			*tdata = root->data;
			return TRUE;
		} 
		else if (strcmp(root->key, name) > 0)
		{
			return search_symtab_fce(root->lptr, name, tdata);
		}
		else{
			return search_symtab_fce(root->rptr, name, tdata);
		}
	}
}

void free_symtab_fce_all(Ttnode_fce_ptr* root){
	if (*root != NULL)
	{
		free_symtab_fce_all(&(*root)->lptr);
		free_symtab_fce_all(&(*root)->rptr);
		free_symtab_id(&(*root)->variables);
		free((*root)->data.param_list);
		free((*root)->key);
		free(*root);
		*root = NULL;
	}
}

///calling 
// if (search_symtab_id(get_symtab_fces_variables(&root,FCE NAME)),VARIABLE NAME, &DATA))
int get_symtab_fces_variables( Ttnode_fce_ptr root, char* name, Ttnode_id_ptr* var){
	
	if (root == NULL)
	{
		return FALSE;
	}
	else{
		if (!(strcmp(root->key, name)))
		{
			*var = root->variables;
			return TRUE;
		} 
		else if (strcmp(root->key, name) > 0)
		{
			return get_symtab_fces_variables(root->lptr, name, var);
		}
		else{
			return get_symtab_fces_variables(root->rptr, name, var);
		}
	}	
}

//////////////////////////////////////////////////////
// Variables tree
int first_insert_id(Ttnode_fce_ptr root,char* name_fce, char* name_v, Tdata_id* tdata){
	
	if (root == NULL)
	{
		return FALSE;
	}
	else{
		if (!(strcmp(root->key, name_fce)))
		{
			if ((root->variables = malloc(sizeof(struct tnode_id))) == NULL)
				exit(-1);
			if ((root->variables->key = malloc(sizeof(char)*(strlen(name_v)+7))) == NULL)
				exit(-1);
			root->variables->key = strcpy(root->variables->key,name_v);
			root->variables->data = *tdata;
			root->variables->lptr = NULL;
			root->variables->rptr = NULL;
			
			return TRUE;
		} 
		else if (strcmp(root->key, name_fce) > 0)
		{
			return first_insert_id(root->lptr, name_fce, name_v, tdata);
		}
		else{
			return first_insert_id(root->rptr, name_fce, name_v, tdata);
		}
	}	
}
int insert_symtab_id(Ttnode_id_ptr* root, char* name, Tdata_id* tdata){
	if (*root == NULL)
	{
		if((*root = malloc(sizeof(struct tnode_id))) == NULL)
			exit(-1);

		if (((*root)->key = malloc(sizeof(char)*(strlen(name)+7))) == NULL)
			exit(-1);

		(*root)->key = strcpy((*root)->key, name);
		(*root)->data = *tdata;
		(*root)->lptr = NULL;
		(*root)->rptr = NULL;
		return TRUE;
	}
	else{
		if (strcmp((*root)->key, name) > 0)
			return insert_symtab_id(&(*root)->lptr, name, tdata);
		else if(strcmp((*root)->key, name) < 0)
			return insert_symtab_id(&(*root)->rptr, name, tdata);
		else{
			return FALSE;
		}		
	}
}
// TRUE or FALSE if true data returned in Tdata_id* data
int  search_symtab_id(Ttnode_id_ptr root ,char* name, Tdata_id* tdata){
	if (root == NULL)
	{
		return FALSE;
	}
	else{
		if (!(strcmp(root->key, name)))
		{
			*tdata = root->data;
			return TRUE;
		} 
		else if (strcmp(root->key, name) > 0)
		{
			return search_symtab_id(root->lptr, name, tdata);
		}
		else{
			return search_symtab_id(root->rptr, name, tdata);
		}
	}
}

void free_symtab_id(Ttnode_id_ptr* root){
	
	if (*root != NULL)
	{
		free_symtab_id(&(*root)->lptr);
		free_symtab_id(&(*root)->rptr);
		free((*root)->key);
		free(*root);
		*root = NULL;
	}
}
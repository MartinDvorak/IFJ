
#include "symbtab.h"

// Function tree
void init_tree(Ttnode_ptr* root){
	*root = NULL;
}

int insert_tree(Ttnode_ptr* root, char* name, Tdata* data){
	if (*root == NULL)
	{
		if ((*root = malloc(sizeof(struct tnode))) == NULL)
			exit(-1);// magical constant 7 for save
		if (((*root)->key = malloc(sizeof(char)*(strlen(name)+7))) == NULL) 
			exit(-1);
		(*root)->key = strcpy((*root)->key ,name);
		(*root)->data = *data;
		(*root)->lptr = NULL;
		(*root)->rptr = NULL;
		return TRUE;
	}
	else{
		if (strcmp(((*root)->key),name) > 0)
			return insert_tree(&(*root)->lptr, name, data);
		else if (strcmp(((*root)->key),name) < 0)
			return insert_tree(&(*root)->rptr, name, data);
		else{
			return FALSE;
		}
	}
}

// TRUE or FALSE if true data returned in Tdata_fce* data
int  search_tree(Ttnode_ptr root ,char* name, Tdata* tdata){
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
			return search_tree(root->lptr, name, tdata);
		}
		else{
			return search_tree(root->rptr, name, tdata);
		}
	}
}

int ifdefined(Ttnode_ptr root, char* name)
{
	if (root == NULL)
	{
		return FALSE;
	}
	else{
		if (!(strcmp(root->key, name)))
		{
			if (root->data.defined)
				return TRUE;
			return FALSE;
		} 
		else if (strcmp(root->key, name) > 0)
		{
			return ifdefined(root->lptr, name);
		}
		else{
			return ifdefined(root->rptr, name);
		}
	}
}

void free_tree(Ttnode_ptr* root){
	if (*root != NULL)
	{
		free_tree(&(*root)->lptr);
		free_tree(&(*root)->rptr);
		free((*root)->key);
		free_data(&(*root)->data);
		free(*root);
		*root = NULL;
	}
}

void free_data(Tdata* data){
	free(data->param);
}

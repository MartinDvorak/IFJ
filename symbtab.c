

#include "symbtab.h"

// Function tree
void init_tree(Ttnode_ptr* root){
	*root = NULL;
}
/*
Tdata* insert_data( Tdata* data)
{
	Tdata* dest;
	if((dest = malloc(sizeof(Tdata))) == NULL)
		exit(99);
	dest->type = data->type;
	dest->defined = data->defined;
	dest->param =  data->param;
}
*/

int insert_tree(Ttnode_ptr* root, char* name, Tdata* data){
	if (*root == NULL)
	{
		if ((*root = malloc(sizeof(struct tnode))) == NULL)
			exit(-1);// magical constant 7 for save
		if (((*root)->key = malloc(sizeof(char)*(strlen(name)+7))) == NULL) 
			exit(-1);
		(*root)->key = strcpy((*root)->key ,name);
		
		(*root)->data.type = data->type;
		(*root)->data.defined = data->defined;
		(*root)->data.param = data->param;
		
		//(*root)->data = *data;
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

int insert_data_tree(Ttnode_ptr* root, char* name, Tdata* tdata)
{
	if (*root == NULL)
	{
		return FALSE;
	}
	else{
		if (!(strcmp((*root)->key, name)))
		{
			(*root)->data = *tdata;
			return TRUE;
		} 
		else if (strcmp((*root)->key, name) > 0)
		{
			return insert_data_tree(&(*root)->lptr, name, tdata);
		}
		else{
			return insert_data_tree(&(*root)->rptr, name, tdata);
		}
	}
}

int insert_define_tree(Ttnode_ptr* root, char* name,int search, int set)
{
	if (*root == NULL)
	{
		return FALSE;
	}
	else{
		if (!(strcmp((*root)->key, name)))
		{
			if((*root)->data.defined == search)
				return FALSE;
			(*root)->data.defined = set;
			return TRUE;
		} 
		else if (strcmp((*root)->key, name) > 0)
		{
			return insert_define_tree(&(*root)->lptr, name,search,set);
		}
		else{
			return insert_define_tree(&(*root)->rptr, name,search,set);
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
int search_in_pre_order(Ttnode_ptr root)
{
	if(root == NULL)
	{
		return FALSE;
	}
	else{
		if(root->data.defined == -1)
			return TRUE;
		return search_in_pre_order(root->lptr) || search_in_pre_order(root->lptr);
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
/* 
STRUCT data
int type = {int, string, float} // in function return type? 
int defined = {TRUE, FALSE} inicialization = FALSE
char *param = {integer = 'i'; string = 's'; float = 'f'} init = NULL
*/
#ifndef TTREE
typedef struct data{
	int type;
	int defined;
	char* param; 
}Tdata;

typedef struct tnode{
	char* key; // name 
	Tdata data; 	
	
	struct tnode* lptr;
	struct tnode* rptr;

}*Ttnode_ptr;
#endif
#define TTREE

void init_tree(Ttnode_ptr* root);

// FALSE - already exist
int insert_tree(Ttnode_ptr* root, char* name, Tdata* data);

// TRUE - find 
int  search_tree(Ttnode_ptr root ,char* name, Tdata* tdata);

// TRUE - defined / FALSE - not defined or not exist
int ifdefined(Ttnode_ptr root, char* name);

void free_tree(Ttnode_ptr* root);

void free_data(Tdata* data);

// TRUE - insert new data
int insert_data_tree(Ttnode_ptr* root, char* name, Tdata* data);

// hleda nedefinovane fce data.defined = -1;
int search_in_pre_order(Ttnode_ptr root);

int insert_define_tree(Ttnode_ptr* root, char* name,int search,int set);
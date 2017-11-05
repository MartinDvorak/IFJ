/********************************************
*	xdvora2l	Dvořák Martin 
*	
*	IFJ Symtab
*
*	Implementation using recursion 

*  je vytovřen jeden strom ve kterém jsou uložený fšechny funkce které se volají, 
   nasledně každý uzel tohoto stromu ukazuje mimo jiné na stvůj vlastní  strom, kde
   jsou uloženy všechny lokální proměnné a paramentry fce	

*	vyhedávací klíč je string a je rozřazováni pomocí fce z knihovny string.h
	strcmp(const* char, const* char)	
************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

//informační hodnota pro identifikátory
typedef struct data_id{
	int type;
	int state;
}Tdata_id;

// informace o funkci 
typedef struct data_fce{
	char *param_list;
	int type_return;
	int state;
}Tdata_fce;

// uzel stromu pro identifikátory 
typedef struct tnode_id{
	char* key; // name 
	Tdata_id data; 	
	
	struct tnode_id* lptr;
	struct tnode_id* rptr;

}*Ttnode_id_ptr;

// uzel stromu pro funkce
typedef struct tnode_fce{
	char* key; // name 
	// {i for integer; f for float; s for string; NULL = 0arg} in order
	Tdata_fce data;
	
	//Ttnode_id_ptr * variables;
	struct tnode_id* variables;

	struct tnode_fce* lptr;
	struct tnode_fce* rptr;
}*Ttnode_fce_ptr;


/////////////////////////////////////
// Function tree
void init_symtab_fce(Ttnode_fce_ptr* root);

// vloží nový uzel do stromu fce, pokud už uzel s tímto jménem(klíčem) exituje vrací FALSE
int insert_symtab_fce(Ttnode_fce_ptr* root, char* name, Tdata_fce* tdata);

// vyhledává fci s klíčem v parametru jméno, pokud nalene vrací TRUE a data jsou v ukazateli tdata
// pokud vrátí FALSE je tdata nedefinovaná
int  search_symtab_fce(Ttnode_fce_ptr root, char* name, Tdata_fce* tdata);

// uvolnění kompletně celé tabluky!! tzn. i tabulky pro identifikátory 
void free_symtab_fce_all(Ttnode_fce_ptr* root);

/*
fce hledá podle parametru name fci se stejným klíčem a vrací ukazatel na strom identifikátorů

pokud vratí TRUE je nalezen uzel a proměnná var je napněna ukazatelem na ukazatel začátku stromu pro id.
pokud vrací FALSE je nedefinovaná 
pro více viz tree.c
*/
int get_symtab_fces_variables( Ttnode_fce_ptr root, char* name, Ttnode_id_ptr* var);


/*
pokud get_symtab_fces_variables vratí NULL v parametru. pak se musí zavolat tato fce na vložení uzlu do tromu identifikátorů
nejdříve vyhledá fci podle name_fce a následně vloží proměnou name_v s daty tdata
*/
int first_insert_id(Ttnode_fce_ptr root,char* name_fce, char* name_v, Tdata_id* tdata);

/*
stejná fce jako insert_symtab_fce aokrát nad stromem id.
nutné získat ptr pomocí fce get_symtab_fces_variables
*/
int insert_symtab_id(Ttnode_id_ptr* root, char* name, Tdata_id* tdata);

/*
vyhedá v stromu identifikátorů položku s stringem name 
TRUE - nalezlo se 
FALSE - nenalezlo se
*/
int  search_symtab_id(Ttnode_id_ptr root ,char* name, Tdata_id* tdata);

void free_symtab_id(Ttnode_id_ptr* root);





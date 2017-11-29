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

#ifndef SYMBTAB_H
#define SYMBTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#ifndef TTREE
#define TTREE
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


/**
 * @brief      unicialilizace stromu
 *
 * @param      root  ukzatel na kořen stromu
 */
void init_tree(Ttnode_ptr* root);



/**
 * @brief      vkládá item pomocí klíče v parametru name a vloží data z
 *             parametru data. Na první vhodný prázdný list
 *
 * @param      root  ukazatel na kořen stromu
 * @param      name  klíč do vyhledávacího stromu
 * @param      data  ukazatel na data, která se budou vkládat
 *
 * @return     pokud se vše povedlo vrací TRUE, jinak FALSE i u redefinice
 */
int insert_tree(Ttnode_ptr* root, char* name, Tdata* data);




/**
 * @brief      prohledává strom a hledá prvek s klíčem name
 *
 * @param[in]  root   ukazatel na kořen stromu, který se má prohledat
 * @param      name   klíč podle kterého se bude vyhledávat
 * @param      tdata  ukazatel kam se má uložit vyhledaný prvek
 *
 * @return     pokud je prvek nalezen vrací TRUE jinak FALSE
 */
int  search_tree(Ttnode_ptr root ,char* name, Tdata* tdata);



/**
 * @brief      zjištuje zda daný zápis ve vyhledávacím stomě je definovaný
 *
 * @param[in]  root  ukazatel na strom, ve kteém se bude hledat
 * @param      name  klíč podle, kterého se vyhledává
 *
 * @return     vrací TRUE pokud je prvek nalezen a definován jinak vrací FALSE
 */
int ifdefined(Ttnode_ptr root, char* name);



/**
 * @brief      uvolňuje celý strom z paměti
 *
 * @param      root  ukazatel na strom
 */
void free_tree(Ttnode_ptr* root);



/**
 * @brief      uvolňuje data z udčitého uzlu stromu
 *
 * @param      data  ukazatel na data, které se mají uvolnit
 */
void free_data(Tdata* data);



/**
 * @brief      vkládá data do již exitujíciho uzlu ve vyhledávacím stromě
 *
 * @param      root  ukazatel na kořen stormu
 * @param      name  klíč podle, kterého se bude vyhledávat
 * @param      data  užitečný obsah, který se má vložit
 *
 * @return     vrací TRUE pokud se povedlo aktualizovat data. jinak FALSE
 */
int insert_data_tree(Ttnode_ptr* root, char* name, Tdata* data);



/**
 * @brief      hledá po skončení celé syntaktické a sématické kontroly fce,
 *             které byli volány ale nejdou definovány. strom je prohledávám v
 *             pre_order prohledávání
 *
 * @param[in]  root  ukazatel na kořen stromu
 *
 * @return     vrací TRUE pokud se taková fce našla, jinak FALSE
 */
int search_in_pre_order(Ttnode_ptr root);




/**
 * @brief      hledá ve vyhledávácím stromě záznam s klíčem name. Pokud je
 *             záznam nalezen je zkontrolována poližka dat .define jestli se
 *             nerovná parametr searcha následně nastavena pomocí parametru set
 *
 * @param      root    ukazatel na strom
 * @param      name    klíč, která se vyhledává
 * @param[in]  search  pokud se data.define uzlu rovnají tomuto parametru je
 *                     vracena false
 * @param[in]  set     nastavuje data.define na tuto hodnotu
 *
 * @return     vrací TRUE pokud proběhlo vče v pořádku jinak FALSE
 */
int insert_define_tree(Ttnode_ptr* root, char* name,int search,int set);

#endif
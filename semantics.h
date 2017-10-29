
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //for modf
#include "symbtab/symbtab.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define INCOMPATIBLE_TYPES 10
#define SAME_TYPES 11
#define COMPATIBLE_TYPES 12


//hodnoty jsme zatim napsal stejne jako ma meloun u scanneru
#define INTEGER 22
#define DOUBLE 15
#define STRING 28

#define NONE 0
#define DECLARED 1
#define DEFINED 2



/*EXPRESSIONS*****************************************************/

//section znamena funkci, hlavni telo programu atd.

//hledani promenne, NULL pokud nenalezena
Tdata_id* lookup_var(Ttnode_fce_ptr* root, char* name, char* section_name);

//ziska stav promenne, vraci NONE pokud nedefinovana, DEFINED pokud definovana
int get_state_var(Tdata_id* var_data);

//ziska typ promenne, INTEGER, DOUBLE, nebo STRING
int get_type_var(Tdata_id* var_data);

//ziska hodnotu promenne, podle typu
int get_value_var_INTEGER(Tdata_id* var_data);
double get_value_var_DOUBLE(Tdata_id* var_data);
char* get_value_var_STRING(Tdata_id* var_data);

/*porovna typy dvou identifikatoru
 *vraci INCOMPATIBLE_TYPES, SAME_TYPES nebo COMPATIBLE_TYPES
 *0 pokud error (nenalezena promenna)
 */
int match_types(Ttnode_fce_ptr* root, char* id1, char* id2, char* section_name);

//pretypovani doublu na integer
void cast_double_to_int(Tdata_id* var_data);

//pretypovani integeru na double
void cast_int_to_double(Tdata_id* var_data);



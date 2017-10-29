#include "semantics.h"






/*hledani lokalni promenne v tabulce symbolu*/
Tdata_id* lookup_var(Ttnode_fce_ptr* root, char* name, char* section_name){

	Tdata_id* searched_var = NULL;


		//hledani stromu lokalnich promennych funkce
		Ttnode_id_ptr var;
		int result;
		if((result = get_symtab_fces_variables(*root, section_name, &var)) == 0){

			return NULL;
		}
	
		//hledani promenne
		Tdata_id* var_data = NULL;
		if(search_symtab_id(var, name, var_data) == 0){

			return NULL;
		}	

	return searched_var;
}

//vraci stav promenne
int get_state_var(Tdata_id* var_data){

	return var_data->state;
}



//vraci typ promenne
int get_type_var(Tdata_id* var_data){

	return var_data->type; 
}



//vraci hodnotu integeru
int get_value_var_INTEGER(Tdata_id* var_data){

	return var_data->INTvalue;
}



//vraci hodnotu doublu
double get_value_var_DOUBLE(Tdata_id* var_data){

	return var_data->DOUBLEvalue;
}



//vraci hodnotu stringu
char* get_value_var_STRING(Tdata_id* var_data){

	return var_data->STRINGvalue;
}



int match_types(Ttnode_fce_ptr* root, char* id1, char* id2, char* section_name){

	Tdata_id* id1_data;
	Tdata_id* id2_data;

	if((id1_data = lookup_var(root, id1, section_name)) == NULL){
		return 0;
	}

	if((id2_data = lookup_var(root, id2, section_name)) == NULL){
		return 0;
	}

	//datove typy jsou stejne 
	if (id1_data->type == id2_data->type){
		return SAME_TYPES;
	}

	//datove typy jsou kompatibilni, lze pretypovat
	if ((id1_data->type == INTEGER && id2_data->type == DOUBLE) || 
		(id1_data->type == DOUBLE && id2_data->type == INTEGER)){

		return COMPATIBLE_TYPES;
	}

	//datove typy nejsou kompatibilni --> ERROR 
	return INCOMPATIBLE_TYPES;
}


//pretypovani doublu na integer
void cast_double_to_int(Tdata_id* var_data){

	double value = var_data->DOUBLEvalue;
	double intpart;
	double fractpart;

	//deli double na integer a desetinnou cast
	fractpart = modf (value , &intpart);

	int casted_value = (int) intpart;
	if ((fractpart >= 0.5) && (casted_value % 2 != 0)){
		//reseni preference na sude
		casted_value++;
	}

	var_data->INTvalue = casted_value;
	var_data->type = INTEGER;
}


//pretypovani integeru na double
void cast_int_to_double(Tdata_id* var_data){

	int value = var_data->INTvalue;
	var_data->DOUBLEvalue = (double)value;
	var_data->type = DOUBLE;
}




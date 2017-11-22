/****IF THEN ELSE STATEMENT************************************************************************/

//skok na else vetev, bool hodnota vyhodnoceni vyrazu je na vrcholu datoveho zasobniku, vycisti ho 
void codegen_if_cond_jump(int actual_if_id){

	printf("DEFVAR LF@$if_cond_state_%d\n", actual_if_id);
	printf("POPS LF@$if_cond_state_%d\n", actual_if_id);
	printf("JUMPIFNEQ $else_branch_%d LF@$if_cond_state_%d bool@true\n", actual_if_id, actual_if_id);
	printf("CLEARS\n");
}

//vytvori skok na konec if-st za then vetvi a vytvori label pro skok na else vetev
void codegen_else_label(int actual_if_id){

	printf("JUMP $end_if_%d\n", actual_if_id);
	printf("LABEL $else_branch_%d\n", actual_if_id);

}

//vygeneruje pouze label pro skok na konec if-statementu
void codegen_if_end_label(int actual_if_id){

	printf("LABEL $end_if_%d\n", actual_if_id);
}


/****WHILE LOOP***********************************************************************************/

//nadefinuje promennou pro skok ven z cyklu a nevesti pro navrat na vyhodnoceni podminky
void codegen_loop_top_label(int actual_loop_id){

	printf("DEFVAR LF@loop_state_%d\n", actual_loop_id);
	printf("LABEL $loop_top_%d\n", actual_loop_id);
}

void codegen_loop_cond(int actual_loop_id){

	//ziskani vyhodnoceni podminky ze zasobniku
	printf("POPS LF@loop_state_%d\n", actual_loop_id);
	printf("CLEARS\n");	

	printf("JUMPIFNEQ $loop_end_%d LF@loop_state_%d bool@true\n", actual_loop_id, actual_loop_id);
}

void codegen_loop_end(int actual_loop_id){

	printf("JUMP $loop_top_%d\n", actual_loop_id);
	printf("LABEL $loop_end_%d\n", actual_loop_id);
}


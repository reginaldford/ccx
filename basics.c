#include "cx.h"


WAIT_PRIMITIVE(cp)
{
	type_check(TAG_CONTEXT);
	int obj_size = size_of_obj(input);
	void* new_obj = cx_malloc(obj_size);
	memcpy(new_obj,input,obj_size);
	save_continuum();
	schedule_next(new_obj);
	pop(wait_stack);
}

WAIT_PRIMITIVE(evnul)
{
	if(input!=NULL)
	{
		type_check(TAG_CD);
		save_continuum();
		schedule_next(((cd*)input)->content);
		current_scope = ((cd*)input)->scope;
	}
	pop(wait_stack);
}

WAIT_PRIMITIVE(eval)
{
	if(input != NULL)
	{
		type_check(TAG_CD);
		schedule_next( make_stub((cd*)input) );
		current_scope = ((cd*)input)->scope;
	}
	pop(wait_stack);
}

WAIT_PRIMITIVE(obj_switch)
{
	type_check(TAG_CD);
	ccell* content = ((cd*)input)->content;
	symbol* q =content->car;
	ccell* result_cell = content->cdr;
	if( q->id != cx_true->id )
		result_cell = result_cell->cdr;
	schedule_next(result_cell->car);
	pop(wait_stack);
}

WAIT_PRIMITIVE(apply_if)
{
	pop(wait_stack);
}

WAIT_PRIMITIVE(exit)
{
	type_check(TAG_INT);
	int exit_code = UNTAG_CX_INT(input);
	pop(wait_stack);
	debug("\n");
	free(universe);
	exit(exit_code);
}

void prim_inp(void)
{
	schedule_next(peek(input_stack));
}

WAIT_PRIMITIVE(first)
{
	type_check(TAG_CD);
	schedule_next( (((cd*)input)->content)->car) ;
	pop(wait_stack);
}

WAIT_PRIMITIVE(rest)
{
	type_check(TAG_CD);
	if(input == cx_null)
		{
			schedule_next(cx_null);
			pop(wait_stack);
			return;
		}
	int col_tag = ((cd*)input)->col_tag;
	void* rest = (ccell*) (((cd*)input)->content)->cdr;
	if(rest == cx_null)
		schedule_next(cx_null);
	else
		schedule_next(make_cd(col_tag,current_scope,rest));
	pop(wait_stack);
}

WAIT_PRIMITIVE(equal)
{
	type_check(TAG_CD);
	ccell* current = ((cd*)input)->content;
	void* el1 = current->car;
	void* el2 = ((ccell*)current->cdr)->car;
	if(el1 == el2){ schedule_next(cx_true); }
	else{ schedule_next(cx_false); }
	pop(wait_stack);
}

WAIT_PRIMITIVE(ifte)
{
	type_check(TAG_CD);
	ccell* content = ((cd*)input)->content;
	symbol* q =content->car;
	ccell* result_cell = content->cdr;
	if( q->id != cx_true->id )
		result_cell = result_cell->cdr;
	schedule_next(((cd*)result_cell->car)->content);
	pop(wait_stack);
}

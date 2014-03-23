#include "cx.h"

ccell *cons(void *car_val, void *cdr_val)
{
	ccell *make_cell = (ccell*) cx_malloc(sizeof(ccell));
	make_cell->tag = TAG_CCELL;
	make_cell->car = car_val;
	make_cell->cdr = cdr_val;
	return make_cell;
}

ccell *add_link(ccell *lastNode, void *value)
{
	ccell *newNode = cons(value,NULL);
	lastNode->cdr = newNode;
	return newNode;
}

ccell *find_end(ccell *start)
{
	if(start==NULL)
		return NULL;
	while(start->cdr != NULL)
		start = start->cdr;
	return start;
}

WAIT_PRIMITIVE(cons)
{
	ccell* iter = ((cd*)input)->content;
	type_check(TAG_CD);
	ccell* new_cell = cons(iter->car, ((ccell*)iter->cdr)->car);
	schedule_next(make_cd(LIST,NULL,new_cell));
	pop(wait_stack);
}

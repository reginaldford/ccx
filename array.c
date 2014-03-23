#include "cx.h"

cx_array* make_array(int size)
{
	cx_array *a = cx_malloc(sizeof(cx_array)+size*sizeof(void*));
	a->tag = TAG_ARRAY;
	a->size = size;
	return a;
}

void** get_array_content(cx_array* a)
{
	return (void**)(a+1);
}

void* get_slot(cx_array* a,int slot)
{
	if(a->size < slot || slot < -1)
	{
		printf("\ncannot get slot %i of an array of size %i", slot,a->size);
		fflush(stdout);
		return 0;
	}
	else
		return get_array_content(a)[slot];

}

void set_slot(cx_array* a, int slot, void* value)
{
	if(value<universe || value > universe+pos)
		debug("wtf?");
	if(a->size < slot)
	{
		printf("\ncannot set slot %i of an array of size %i", slot,a->size);
		fflush(stdout);
	}
	else
		get_array_content(a)[slot]=value;
}

cx_array* add_slot(cx_array* cxa,int slot,void* value)
{
	cx_array* new_array = cx_malloc(sizeof(cx_array)+sizeof(void*)*cxa->size+1);
	memcpy(new_array,cxa,sizeof(cx_array)+sizeof(void*)*(slot));
	new_array->size ++;
	set_slot(new_array,slot,value);
	memcpy(new_array + sizeof(cx_array) + sizeof(void*) * (slot + 1),
               cxa + sizeof(cx_array) + sizeof(void*) * slot,
               sizeof(void*) * (cxa->size - slot));
	return new_array;
}

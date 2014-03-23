#include "cx.h"

void check_gc(int malloc_size)
{
	if(!can_fit_more(malloc_size))
	{
		gc();
		while(!can_fit_more(malloc_size))
			grow_universe();
	}
}

void grow_universe(void)
{
	universe_size += 1 MB ;
	gc();
}

bool can_fit_more(int how_much)
{
	return pos+how_much<=universe_size;
}

void copy_roots(void)
{
	MEET_OBJ(prim_gc);
	MEET_OBJ(input);
	MEET_OBJ(cx_true);
	MEET_OBJ(cx_false);
	MEET_OBJ(cx_null);
	MEET_OBJ(current);
	MEET_OBJ(callstack);
	MEET_OBJ(wait_stack);
	MEET_OBJ(input_stack);
	MEET_OBJ(collections);
	MEET_OBJ(lobby);
	MEET_OBJ(trigger);
	MEET_OBJ(collector);
	MEET_OBJ(inper);
	MEET_OBJ(symbol_table);	
	MEET_OBJ(current_scope);
}

void prep_new_space(void)
{
	new_space = malloc(universe_size);
	new_space_pos = 0;
}

void switch_spaces(void)
{
	free(universe);
	pos = new_space_pos;
	universe = new_space;
}

void gc(void)
{
	prep_new_space();
	copy_roots();
	inflate_new_space();
	switch_spaces();
}

void* new_space_malloc(int bytes)
{
	int current_pos = new_space_pos;
	new_space_pos+=bytes;
	return new_space+current_pos;
}

void* to_new_space(void* obj)
{
	int obj_size = ALIGN(size_of_obj(obj));
	gco* the_gco = (gco*)obj;
	void* new_home = new_space_malloc(obj_size);
	memcpy(new_home,obj,obj_size);
	the_gco->tag = TAG_GCO ;
	the_gco->new_address = new_home;
	return new_home;
}

void* meet_obj(void* obj)
{
	if(obj == NULL)
		return NULL;
	else if(tag_of(obj) == TAG_INT)
		return obj;
	else if(tag_of(obj) == TAG_GCO)
		return((gco*)obj)->new_address;
	else
		return to_new_space(obj);
}

void inflate_new_space(void)
{
	int ns_scan_pos = 0;
	while(ns_scan_pos < new_space_pos)
	{
		void * obj = new_space+ns_scan_pos;
		switch(tag_of(obj))
		{
		case TAG_BIND:
			MEET_OBJ(((binding*)obj)->value);
			break;
		case TAG_STACK:
		{
			stack* s = (stack*)obj;
			void** the_content = get_stack_content(s);
			int x,height = s->height;
			for(x=0;x<height;x++)
				MEET_OBJ(the_content[x]);
		}
			break;
		case TAG_CD:
			MEET_OBJ(((cd*)obj)->content);
			MEET_OBJ(((cd*)obj)->scope);
			break;
		case TAG_I_S:
			if(tag_of(((inp_stub*)obj)->func) == TAG_CD)
				MEET_OBJ(((inp_stub*)obj)->func);
			else if(tag_of(((inp_stub*)obj)->func) != TAG_PRIMITIVE)
				MEET_OBJ(((inp_stub*)obj)->func);
			break;
		case TAG_CCELL:
			MEET_OBJ(((ccell*)obj)->car);
			MEET_OBJ(((ccell*)obj)->cdr);
			break;
		case TAG_CONTEXT:
			MEET_OBJ(((context*)obj)->content);
			MEET_OBJ(((context*)obj)->parent);
			break;
		case TAG_ARRAY:
			{
				cx_array* cxa = (cx_array*)obj;
				void** the_content = get_array_content(cxa);
				int x,size = cxa->size;
				for(x=0;x<size;x++)
					MEET_OBJ(the_content[x]);
			}
			break;
		case TAG_INT:
		case TAG_PRIMITIVE:
		case TAG_STRING:
		case TAG_SYMBOL:
			break;
		default:
			printf("unknown tag: %i",tag_of(obj));
			ns_scan_pos++;
			break;
		}
		ns_scan_pos+=ALIGN(size_of_obj(obj));
	}
}


#include "cx.h"

void build_universe(int bytes)
{
	bytes= ALIGN(bytes);
	universe = malloc(bytes);
	pos = 0;
}

void* cx_malloc(int bytes)
{
	bytes = ALIGN(bytes);
	int current_pos = pos;
	pos+=bytes;
	return universe+current_pos;

}

void* cx_realloc(void* src, int bytes)
{
	void* new_place = cx_malloc(bytes);
	if(src != NULL)
		memcpy(new_place,src,size_of_obj(src));
	return new_place;
}

int size_of_obj(void * obj)
{
	int tag = tag_of(obj);
	switch(tag)
	{
	case TAG_CD:
		return sizeof(cd);
		break;
	case TAG_PRIMITIVE:
		return sizeof(primitive);
		break;
	case TAG_SYMBOL:
		return sizeof(symbol);
		break;
	case TAG_NIL:
	case TAG_INT:
		return sizeof(void*);
		break;
	case TAG_STRING:
		return sizeof(string)+((string*)obj)->length*sizeof(char);
		break;
	case TAG_I_S:
		return sizeof(inp_stub);
		break;
	case TAG_ARRAY:
		return sizeof(cx_array)+((cx_array*)obj)->size*sizeof(void*);
		break;
	case TAG_CCELL:
		return sizeof(ccell);
		break;
	case TAG_CONTEXT:
		return sizeof(context);
		break;
	case TAG_STACK:
		return sizeof(stack)+((stack*)obj)->capacity*sizeof(void*);
		break;
	case TAG_BIND:
		return sizeof(binding);
		break;
	case TAG_GCO:
		return sizeof(gco);
		break;
	default:
		printf("no size for object with tag: %i\n", tag);
		return 0;
		break;
	}
}

int tag_of(void* obj)
{
	if(obj == NULL)
		return TAG_NIL;
	else if(((int)obj) & 1)
		return TAG_INT;
	else
		return  *((int*)obj);
}

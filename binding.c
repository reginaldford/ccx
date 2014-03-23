#include "cx.h"

binding* make_binding(int id, void* value)
{
	binding* make_bind = cx_malloc(sizeof(binding));
	make_bind->tag = TAG_BIND;
	make_bind->id = id;
	make_bind->value = value;
	return make_bind;
}

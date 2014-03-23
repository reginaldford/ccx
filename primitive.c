#include "cx.h"

primitive* make_prim(void(*funcPtr)(void))
{
	primitive* product = cx_malloc(sizeof(primitive));
	product->tag = TAG_PRIMITIVE;
	product->block = funcPtr;
	return product;
}

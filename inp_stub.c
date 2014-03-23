#include "cx.h"

inp_stub* make_stub(cd* func)
{
	inp_stub* i_s = cx_malloc(sizeof(inp_stub));
	i_s->tag = TAG_I_S;
	i_s->func = func;
	return i_s;
}

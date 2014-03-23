#include "cx.h"

WAIT_PRIMITIVE(register_singletons)
{
	type_check(TAG_CD);
	ccell* current_cell = ((cd*)input)->content;
	cx_true = current_cell->car;
	SET_NEXT_CELL(cx_false);
	SET_NEXT_CELL(cx_null);
	pop(wait_stack);
}

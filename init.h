#define SET_NEXT_CELL(foo)		\
current_cell = current_cell->cdr;	\
foo  = current_cell->car

WAIT_PRIM_PT(register_singletons);


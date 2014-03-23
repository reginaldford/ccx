typedef struct
{
	int tag;
	void *car;
	void *cdr;
} ccell ;

ccell *cons(void *car, void *cdr);
ccell *add_link(ccell *lastNode, void *value);
ccell *find_end(ccell *start);
WAIT_PRIM_PT(cons);

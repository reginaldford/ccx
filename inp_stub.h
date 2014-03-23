typedef struct
{
	int tag;
	cd* func;
}inp_stub;

inp_stub* make_stub(cd* func);

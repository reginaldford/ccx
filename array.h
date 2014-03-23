typedef struct{
	int tag;
	int size;
}cx_array;

cx_array* make_array(int size);
void** get_array_content(cx_array* a);
void* get_slot(cx_array* a,int slot);
void set_slot(cx_array*a, int slot, void* value);
cx_array* add_slot(cx_array* cxa,int slot,void* value);

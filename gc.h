void check_gc(int malloc_size);
bool can_fit_more(int how_much);
void gc(void);
void* to_new_space(void* obj);
void* new_space_malloc(int bytes);
void* meet_obj(void* obj);
void inflate_new_space(void);
void grow_universe(void);
void copy_roots(void);
void switch_spaces(void);
void prep_new_space(void);

#define MEET_OBJ(obj) obj = meet_obj(obj)

typedef struct
{
	int tag;
	void* new_address;
} gco ;

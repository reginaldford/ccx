typedef struct
{
	int tag;
	int capacity;
	int height;
	int lin_growth;
	int exp_growth;
} stack;

void** get_stack_content(stack* s);
stack* make_stack(int capacity,int lin_growth, int exp_growth);
void push(stack* s, void* obj);
stack* grow_stack(stack* s);
void* pop(stack* s);
void* peek(stack* s);
void* swap(stack* s, void* obj);
void swap_top_two(stack* s);
void push_all(stack* s, void** objs, int num_objs);

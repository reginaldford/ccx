typedef struct{
	int tag;
	cx_array* content;
	void* parent;
} context;

void make(int id, void* value, context* context);
void set(int id, void* value, context* context);
void* get(int id, context* context);
binding* find_binding(int id, context* cx);
context* make_context();
binding* find_binding_delegating(int id, context* cx);
WAIT_PRIM_PT(defun);
WAIT_PRIM_PT(defnul);
WAIT_PRIM_PT(make);
WAIT_PRIM_PT(set);

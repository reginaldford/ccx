typedef struct
{
	int tag;
	int id;
	void* value;
} binding;

binding* make_binding(int id, void* value);

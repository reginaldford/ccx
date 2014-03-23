typedef struct{
	int tag;
	int id;
	int meta_level;
} symbol;

symbol *make_symbol(int id, int meta_level);
int get_sid_or_null(char *sym_name);
int get_sid(char *sym_name);
int register_new_symbol(char *sym_name);
char* get_symbol_name(int id);

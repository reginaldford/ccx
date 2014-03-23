#include "cx.h"

symbol *make_symbol(int id, int meta_level)
{
	symbol *make_symbol = cx_malloc(sizeof(symbol));
	make_symbol->tag=TAG_SYMBOL;
	make_symbol->id=id;
	make_symbol->meta_level=meta_level;
	return make_symbol;
}

/* gets id or gives NULL */
int get_sid_or_null(char *sym_name)
{
	ccell *current = symbol_table;
	int x;
	for(x = 1; x<=symbol_table_size;x++)
		if(strcmp(get_str_content((string*)current->car), sym_name)==0)
			return x;
		else
			current=current->cdr;
	return 0;
}

/* returns id, already existant or not */
int get_sid(char *sym_name)
{
	int id = get_sid_or_null(sym_name);
	if(id == 0)
		return register_new_symbol(sym_name);
	else
		return id;
}

int register_new_symbol(char *sym_name)
{
	ccell* end = find_end(symbol_table);
	if(end != NULL)
		end->cdr = cons(make_string(sym_name),NULL);
	else
		symbol_table = cons(make_string(sym_name),NULL);
	symbol_table_size++;
	return symbol_table_size;
}

char* get_symbol_name(int id)
{
	ccell* current = symbol_table;
	int x;
	for(x=id;x>1;x--)
		current = current->cdr;
	return (char*)get_str_content((string*)current->car);
}

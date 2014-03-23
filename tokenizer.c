#include "cx.h"

int meta_levels(char *sym_name)
{
	char current_char = *sym_name;
	int count =0;
	while(current_char==':')
	{
		count++;
		current_char = sym_name[count];
	}
	return count;
}

void* make_token(char *word)
{
	if(isdigit(*word) || ((*word=='-'|| *word=='+')&& isdigit(word[1])))
		return (void*)MAKE_CX_INT(atoi(word));
	else
	{
		int metalevel = meta_levels(word);
		int id = get_sid(&(word[metalevel]));
		return make_symbol(id,metalevel);
	}
}

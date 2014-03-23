#include "cx.h"
void save_image(void)
{
	int index=0;
	printf("\n\n\n\n\n");
	while(index<pos)
	{
		input = universe+index;
		print_node();
		printf("\n");
		index+=size_of_obj(input);
	}
}

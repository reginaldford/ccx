#include "cx.h"

void sub_prim_collector(void)
{
	if(peek(collections)==NULL)
	{
		ccell* first_cell = cons(input,NULL);
		swap(collections,first_cell);
		push(collections,first_cell);
	}
	else
	{
		ccell* new = cons( input , NULL);
		((ccell*)peek(collections))->cdr = new;
		swap(collections,new);
	}
}

void sub_prim_trigger(void)
{
	pop(wait_stack);
	pop(collections);
	input = make_cd(LITLIST,current_scope,pop(collections));
	execute_waiter();
}

void prim_wait_killer(void)
{
	pop(wait_stack);
}

void prim_function_end(void)
{
	pop(input_stack);
	pop(wait_stack);
}


WAIT_PRIMITIVE(print)
{
	print_node();
	pop(wait_stack);
}

void prim_print_mode(void)
{
	push(wait_stack,make_prim(&print_node));
}

void print_node(void)
{
	int tag = tag_of(input);
	switch(tag)
	{
		case TAG_ARRAY:
			break;
		case TAG_CD:
			print_cd();
			break;
		case TAG_PRIMITIVE:
			printf("<<prim>> ");
			break;
		case TAG_SYMBOL:
			print_symbol();
			break;
		case TAG_INT:
			printf("%i ",UNTAG_CX_INT(input));
			break;
		case TAG_STRING:
			printf("\"");
			input = unformat_string(input);
			print_sized_string(get_str_content((string*)input),((string*)input)->length);
			printf("\" ");
			break;
		case TAG_CONTEXT:
			printf("<context> ");
			break;
		case TAG_STACK:
			printf("<stack> ");
			break;
		case TAG_BIND:
			printf("<bind> ");
			break;
		case TAG_I_S:
			printf("input_stub");
			break;
		case TAG_GCO:
			printf("<gco>");
		case TAG_NIL:
			printf("[ ] ");
			break;
		default:
			printf("tag:%i ",tag);
			break;
	}
	fflush(stdout);
}

void print_symbol(void)
{
	int x;
	for(x = ((symbol*)input)->meta_level;x>0;x--)
		printf(":");
	printf("%s ",get_symbol_name(((symbol*)input)->id));
}

void print_sized_string(char* str, int size)
{
	int x;
	for(x=0;x<size;x++)
		printf("%c",str[x]);
}

WAIT_PRIMITIVE(print_string)
{
	type_check(TAG_STRING);
	string* str = (string*) input;
	print_sized_string(get_str_content(str),str->length);
	fflush(stdout);
	pop(wait_stack);
}

void print_cd(void)
{
	cd* the_cd = ((cd*)input);
	ccell* pos = the_cd->content;
	bool is_list = the_cd->col_tag == LITLIST;
	if(is_list)
		printf("[ ");
	else
		printf("{ ");
	while(pos != NULL)
	{
		input = pos->car;
		print_node();
		pos=pos->cdr;
	}
	if(is_list)
		printf("] ");
	else
		printf("} ");
	input = the_cd;
}

void prim_parse(void)
{
	push(wait_stack,make_prim(&parse_string));
}

void prim_read_line(void)
{
	int final_size,input_size = 100;
	char* the_string = malloc(input_size);
	fgets(the_string,input_size,stdin);
	final_size = strlen(the_string);
	the_string[final_size] = '\0';
	schedule_next(make_string(the_string));
}

WAIT_PRIMITIVE(format)
{
	type_check(TAG_STRING);
	schedule_next(format_string((string*)input));
	pop(wait_stack);
}

#include "cx.h"

void execute_waiter(void)
{
	if(wait_stack->height > 0)
	{
		void * the_waiter = peek(wait_stack);
		if(tag_of(the_waiter) == TAG_CD)
		{
			void* the_scope = ((cd*)the_waiter)->scope;
			current_scope = cx_realloc(the_scope,size_of_obj(the_scope));
			/* ^^ we want a COPY ! */
			schedule_next(inper);
			schedule_next(((cd*)the_waiter)->content);
			pop(wait_stack);
			push(input_stack,input);
		}
		else
			((primitive*)the_waiter)->block();
	}
	else
	{
		debug("\nError: wait stack underflow. input: ");
		print_node();
	}
}

void kill_inp(void)
{
	pop(input_stack);
}

void save_continuum(void)
{
	if(tag_of(current) == TAG_CCELL)
		if(current->cdr != NULL)
			push(callstack,current->cdr);
	current = NULL;
}


void schedule_next(void* next_thing)
{
	save_continuum();
	push(callstack,next_thing);
}

void type_check(int type)
{
	if(type == TAG_CD)
	{
		if(tag_of(input) != TAG_CD && input != cx_null)
			debug("type mismath!");
	}
	else
		if(tag_of(input) != type)
		debug("type mismatch!");
	/*TODO: jump_back and clean up */
}

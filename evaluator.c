#include "cx.h"

void next_node(void)
{
	if(tag_of(current) != TAG_CCELL || current->cdr == NULL)
	{
		current = pop(callstack);
		if(tag_of(current)==TAG_CCELL)
			input = current->car;
		else
			input = current;
	}
	else
	{
		current = current->cdr;
		input = current->car;
	}
}

void evaluate(void)
{
	schedule_next(((cd*)input)->content);
	while(true) eval_current();
}

void eval_current(void)
{
	int tag;
	next_node();
	tag = tag_of(input);
	switch(tag)
	{
		case TAG_CD:
			eval_cd();
			break;
		case TAG_PRIMITIVE:
			((primitive*)input)->block();
			break;
		case TAG_SYMBOL:
			eval_symbol();
			break;
		case TAG_INT:
		case TAG_ARRAY:
		case TAG_STRING:
		case TAG_NIL:
		case TAG_CONTEXT:
			execute_waiter();
			break;
		case TAG_I_S:
			push(wait_stack,((inp_stub*)input)->func);
			break;
		default:
			printf("evaluated object of unknown tag:%i ",tag);
			break;
	}
	check_gc(0.5 KB);
}

void eval_cd(void)
{
	int tag = ((cd*)input)->col_tag;
	switch(tag)
	{
	case LIVEQ:
		schedule_next(((cd*)input)->content);
		break;
	case LIST:
		schedule_next(trigger);
		schedule_next(((cd*)input)->content);
		push(collections,NULL);
		push(wait_stack,collector);
		break;
	case LITLIST:
	case QUOTE:
		execute_waiter();
		break;
	default:
		debug("unknown col... ");
		break;
	}
}

void eval_symbol(void)
{
	symbol* the_sym = (symbol*)input;
	if(the_sym->meta_level == 0)
	{
		binding* the_binding = find_binding_delegating(the_sym->id,current_scope);
		if(the_binding != NULL)
		{
			void* value = the_binding->value;
			if(value != the_sym)
				schedule_next(value);
			else /* the symbol is a singleton */
				execute_waiter(); 
		}
		else
			printf("error:\nsymbol :%s is bound to nothing in this accumulative context", get_symbol_name(the_sym->id));
	}
	else
	{
		input = make_symbol(the_sym->id,the_sym->meta_level-1);
		execute_waiter();
	}
	
}

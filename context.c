#include "cx.h"

context* make_context(void* parent)
{
	context* the_cx = cx_malloc(sizeof(context));
	the_cx->tag = TAG_CONTEXT;
	the_cx->content=make_array(0);
	the_cx->parent = parent;
	return the_cx;
}


void check_sorted(cx_array* cxa)
{
	if(cxa->size>1)
	{
		int x;
		for(x = 1; x<cxa->size; x++)
		{
			if(get_slot(cxa,x)<=get_slot(cxa,x-1))
				debug("UNSORTED !");
		}
	}
}

void make(int id, void* value, context* cx)
{
	binding* b = find_binding(id,cx);
	if(tag_of(b) == TAG_BIND)
		b->value = value;
	else
		cx->content = add_slot(cx->content,UNTAG_CX_INT(b),make_binding(id,value));
	check_sorted(cx->content);
}

/* Returns the binding with this key, or the slot position
that it would have been at , tagged as a CX_INT */
binding* find_binding(int id, context* cx)
{
	cx_array *content = cx->content;
	int old_middle;
	int middle=0;
	int l_bound=0;
	int r_bound = content->size;
	while(l_bound < r_bound)
	{
		old_middle = middle;
		middle = (r_bound+l_bound+1)/2;
		if(middle == old_middle) /*UGH, isnt this the UGLIEST SHIT EVER? */
			return (binding*)MAKE_CX_INT(middle);
		binding* check =  (binding*)get_slot(content,middle-1);
		if(check->id > id)
			r_bound=middle;
		else if(check->id < id)
			l_bound=middle;
		else return check;
	}
		return (binding*)MAKE_CX_INT(middle);
}

binding* find_binding_delegating(int id, context* cx)
{ /* this should not be recursive */
	binding* check = find_binding(id,cx);
	if(tag_of(check) != TAG_BIND)
	{
		if(cx->parent != NULL)
			return find_binding_delegating(id,cx->parent);
		else return NULL;
	}
	else return check;
}

void* get(int id, context* cx)
{	
	return find_binding(id,cx)->value;
}

void set(int id, void* value, context* cx)
{
	binding* b1 = find_binding(id,cx);
	if(tag_of(b1)!=TAG_BIND)
	{
		if(cx->parent)
			set(id,value,(context*)cx->parent);
		else
			printf("Error: %s is bound to nothing in this logical context",get_symbol_name(id));
	}
	else
		b1->value=value;
}

WAIT_PRIMITIVE(set)
{
	type_check(TAG_CD);
	ccell* current_node = ((cd*)input)->content;
	symbol* s = current_node->car;
	void* value;
	current_node = current_node->cdr;
	value = current_node->car;
	set(s->id,value,current_scope);
	pop(wait_stack);
}

WAIT_PRIMITIVE(defun)
{
	type_check(TAG_CD);
	ccell* current_node = ((cd*)input)->content;
	symbol* s = current_node->car;
	cd* func_body;
	current_node = current_node->cdr;
	func_body = current_node->car;
	make(s->id,make_stub(func_body),current_scope);
	pop(wait_stack);
}

WAIT_PRIMITIVE(defnul)
{
	type_check(TAG_CD);
	ccell* current_node = ((cd*)input)->content;
	symbol* s = current_node->car;
	cd* func_body;
	current_node = current_node->cdr;
	func_body = current_node->car;
	func_body->col_tag=LIVEQ;
	make(s->id,func_body,current_scope);
	pop(wait_stack);
}

WAIT_PRIMITIVE(make)
{
	type_check(TAG_CD);
	ccell* current_node = ((cd*)input)->content;
	symbol* s = current_node->car;
	void* value;
	current_node = current_node->cdr;
	value = current_node->car;
	make(s->id,value,current_scope);
	pop(wait_stack);
}

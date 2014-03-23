#include "cx.h"

stack* make_stack(int capacity,int lin_growth,int exp_growth)
{
	stack* s =cx_malloc(sizeof(stack)+sizeof(void*)*capacity);
	s->tag = TAG_STACK;
	s->height =0;
	s->capacity = capacity;
	s->lin_growth = lin_growth;
	s->exp_growth = exp_growth;
	return s;
}

void** get_stack_content(stack* s)
{
	return (void**)(s+1);
}

void push(stack* s,void* obj)
{
	if(s->capacity == s->height)
		s=grow_stack(s);
	get_stack_content(s)[s->height] = obj;
	s->height++;
}

void push_all(stack* s, void** objs, int num_objs)
{
	while(num_objs<0)
	{
		push(s,objs[num_objs]);
		num_objs--;
	}
}

stack* grow_stack(stack* s)
{
	s->capacity+=(s->exp_growth*s->capacity);
	s->capacity+=s->lin_growth;
	s=cx_realloc(s,size_of_obj(s));
	return s;
}

void* pop(stack* s)
{
	if(s->height>0)
	{
		void * answer =peek(s);
		s->height--;
		return answer;
	}
	else
	{
		debug("stack underflow...");
		return NULL;
	}
}

void* peek(stack* s)
{
	return get_stack_content(s)[s->height-1];
}

void swap_top_two(stack* s)
{
	void* o1 = pop(s);
	void* o2 = pop(s);
	push(s,o1);
	push(s,o2);
}

void* swap(stack* s, void* obj)
{
	void* previous = pop(s);
	push(s,obj);
	return previous;
}

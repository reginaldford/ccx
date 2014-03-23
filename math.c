#include "cx.h"

WAIT_PRIMITIVE(add)
{
	int sum=0;
	if(input != NULL)
	{
		type_check(TAG_CD);
		ccell* current_num = ((cd*)input)->content;
		while(current_num != NULL)
		{
			sum+= UNTAG_CX_INT((int)(int)current_num->car);
			current_num=current_num->cdr;
		}
	}
	schedule_next(MAKE_CX_INT(sum));
	pop(wait_stack);
}

WAIT_PRIMITIVE(subtract)
{
	type_check(TAG_CD);
	ccell* current_num = ((cd*)input)->content;
	int sum=UNTAG_CX_INT((int)current_num->car);
	current_num = current_num->cdr;
	while(current_num != NULL)
	{
		sum-= UNTAG_CX_INT((int)current_num->car);
		current_num=current_num->cdr;
	}
	schedule_next(MAKE_CX_INT(sum));
	pop(wait_stack);
}

WAIT_PRIMITIVE(multiply)
{
	int product = 1;
	if(input != NULL)
	{
		type_check(TAG_CD);
		ccell* current_num = ((cd*)input)->content;
		while(current_num != NULL)
		{
			product*= UNTAG_CX_INT((int)current_num->car);
			current_num=current_num->cdr;
		}
	}
	schedule_next(MAKE_CX_INT(product));
	pop(wait_stack);
}

WAIT_PRIMITIVE(divide)
{
	type_check(TAG_CD);
	ccell* current_num = ((cd*)input)->content;
	int sum=UNTAG_CX_INT((int)current_num->car);
	current_num = current_num->cdr;
	while(current_num != NULL)
	{
		sum/= UNTAG_CX_INT((int)current_num->car);
		current_num=current_num->cdr;
	}
	schedule_next(MAKE_CX_INT(sum));
	pop(wait_stack);
}


WAIT_PRIMITIVE(lessorequal)
{
	type_check(TAG_CD);
	ccell* current_node = ((cd*)input)->content;
	int num1 = UNTAG_CX_INT((int)current_node->car);
	int num2;
	current_node = current_node->cdr;
	num2 = UNTAG_CX_INT((int)current_node->car);
	if(num1<=num2)
		schedule_next(cx_true);
	else
		schedule_next(cx_false);
	pop(wait_stack);
}

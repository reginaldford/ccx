#include "cx.h"

void general_err(char* message)
{
	cx_prim_err(message);
}

void process_err(char* message)
{
	error_message = message;
	longjmp(boot_safepoint,1);
}

void cx_prim_err(char* message)
{
	error_message = message;
	longjmp(runtime_safepoint,1);
}

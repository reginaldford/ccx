#include "cx.h"

void build_env(void)
{
	universe_size = 32 MB ;
	/* god made the universe in .7 ms */
	build_universe(universe_size);
	/* the environmental stacks */
	input_stack =make_stack(256,16,0);
	callstack=make_stack(256,16,0);
	wait_stack=make_stack(256,16,0);
	collections=make_stack(256,16,0);
	/* assign environmental things */
	current = cons(NULL,NULL);
	trigger = make_prim(&sub_prim_trigger);
	collector = make_prim(&sub_prim_collector);
	inper = make_prim(&kill_inp);
	w_killer = make_prim(&prim_wait_killer);
	prim_gc = make_prim(&gc);
	symbol_table = (ccell*)NULL;
	symbol_table_size = 0;
	lobby=make_context(NULL);
	current_scope = lobby;
	make(register_new_symbol("new"),make_context(lobby),lobby);
	/* all primitives */
	install_all_prims();
}

void install_prim(char *sym_name,void(*block)(void))
{
	make(register_new_symbol(sym_name),make_prim(block),lobby);
}

void install_all_prims(void)
{
	install_prim("+",&prim_add);
	install_prim("-",&prim_subtract);
	install_prim("*",&prim_multiply);
	install_prim("/",&prim_divide);
	install_prim("parse",&prim_parse);
	install_prim("print",&prim_print_string);
	install_prim("print-node",&prim_print);
	install_prim("print-mode",&prim_print_mode);
	install_prim("eval",&prim_eval);
	install_prim("evnul",&prim_evnul);
	install_prim("?",&prim_obj_switch);
	install_prim("read-line",&prim_read_line);
	install_prim("make",&prim_make);
	install_prim("set",&prim_set);
	install_prim("defun",&prim_defun);
	install_prim("defnul",&prim_defnul);
	install_prim("_",&prim_inp);
	install_prim("register_singletons",&prim_register_singletons);
	install_prim("exit",&prim_exit);
	install_prim("first",&prim_first);
	install_prim("rest",&prim_rest);
	install_prim("is",&prim_equal);
	install_prim("format",&prim_format);
	install_prim("<=",&prim_lessorequal);
	install_prim("if",&prim_ifte);
	install_prim("save_image",&save_image);
	install_prim(".",&prim_wait_killer);
	install_prim("prep",&prep_new_space);
	install_prim("gc",&gc);
	install_prim("cp",&prim_cp);
	install_prim("cons",&prim_cons);
}

void boot_from_file(char* file_name)
{
	if(! setjmp(boot_safepoint))
	{
		printf("ConteX programming kit .40\nCopyright (C) 2005, 2006 Reginald Ford\n");
		printf("view license.txt for details of BSD lisence\n\n");
		parse_file(file_name);
		printf("Evaluating %s ...\n",file_name);
		evaluate();
	}
	else
		printf("Error booting from %s :\n%s\n",file_name,error_message);
}

int main(int argc, char** argv)
{
	build_env();
	if(argc == 1)
		boot_from_file("foundation.cx");
	else
		boot_from_file(argv[1]);
	return 0;
}

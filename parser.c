#include "cx.h"

ccell *current_node;
int *contents;
int lexical_stack_height = 1;
int word_size;
int buffer_size = INIT_BUFFER_SIZE;
char *word_buffer;
char current_char = ' ';
cd **lexical_stack;
bool on_string = false;
bool on_comment = false;
int current_col_size = 0;

void parse_file(char *file_name)
{
	FILE *in_file;
	prepare_to_parse();
	in_file = fopen(file_name,"r");
	if(in_file == NULL)
		general_err("The file does not exist.");
	else
	{
		while(current_char != EOF)
		{
			parse_current_char();
			current_char = getc(in_file);
		}
		finalize_quote();
		input = *lexical_stack;
	}
	fclose(in_file);
}

void parse_string(void)
{
	string* the_str = ((string*)input);
	char* code = get_str_content(the_str);
	int index;
	prepare_to_parse();
	for(index=0;index<the_str->length;index++)
	{
		parse_current_char();
		current_char = code[index];
	}
	finalize_quote();
	schedule_next(*lexical_stack);
	pop(wait_stack);
}

void finalize_quote(void)
{
	check_buffer();
	if(current_col_size ==0)
		*lexical_stack=NULL;
	else
	{
		fix_quote((*lexical_stack)->content);
		((cd*)*lexical_stack)->col_tag = QUOTE;
	}
}

void fix_quote(ccell* c)
{
	ccell* last = c;
	while(c->cdr != NULL)
	{
		last = c;
		c = c->cdr;
	}
	/* Now, I have found the end of this quote.
	If this is that cons(NULL,NULL) I left behind, get rid of it */
	if(c->car == NULL)
		last->cdr = NULL;
}

void parse_current_char(void)
{
	if(on_string)
		check_str_end();
	else if(on_comment)
		check_comment_end();
	else
		parse_norm();
}

void parse_norm(void)
{
	switch(current_char)
	{
		case '\0':
			break;
		case '\r':
		case '\t':
		case '\v':
		case '\f':
		case '\n':
		case ' ':
			check_buffer();
			break;
		case ']':
		case '}':
			check_buffer();
			go_down_level();
			break;
		case '[':
			check_buffer();
			go_up_level(false);
			break;
		case '{':
			check_buffer();
			go_up_level(true);
			break;
		case '(':
			on_comment = true;
			break;
		case '\"':
			on_string = true;
			break;
		default:
			add_to_word();
			break;
	}
}

void check_str_end(void)
{
	if(current_char == '\"')
	{
		current_col_size++;
		convert(MODE_STRING);
		on_string = false;
	}
	else
		add_to_word();
}

void check_comment_end(void)
{
	if(current_char == ')')
		on_comment = false;
}


void add_to_word(void)
{
	if(word_size==buffer_size)
	{
		buffer_size = buffer_size << 1;
		word_buffer=realloc(word_buffer,buffer_size);
	}
	word_buffer[word_size] = current_char;
	word_size++;
}

void go_up_level(bool is_quote)
{
	int col_tag;
	lexical_stack_height++;
	lexical_stack = realloc(lexical_stack,sizeof(void*)*lexical_stack_height);
	/* This is where the tagging takes place: */
	if(is_quote==true)
	{
		col_tag=QUOTE;
		push(scope_stack,make_context(peek(scope_stack)));
	}
	else
		col_tag=LIST;
	ccell* new_cell = cons(NULL,NULL);
	lexical_stack[lexical_stack_height-1] =make_cd(col_tag,peek(scope_stack),new_cell);
	current_col_size = 0;
	current_node = new_cell;
}

void go_down_level(void)
{
	ccell * end;
	lexical_stack_height--;
	end = find_end(((cd *)lexical_stack[lexical_stack_height-1])->content);
	if(current_col_size == 0)
		end->car=NULL;
	else
	{
		fix_quote(lexical_stack[lexical_stack_height]->content); /* fixing occurs */
		end->car=lexical_stack[lexical_stack_height];/* linking occurs */
		if(lexical_stack[lexical_stack_height]->col_tag == QUOTE)
			pop(scope_stack);
	}
	current_col_size = 1;
	end->cdr=cons(NULL,NULL);
	current_node = end->cdr;
	lexical_stack =realloc(lexical_stack,sizeof(void*)*lexical_stack_height);
	
}

void check_buffer(void)
{
	if(word_size>0)
	{
		current_col_size++;
		convert(MODE_NORM);
	}
}

void convert(int the_mode)
{
	word_buffer[word_size]='\0'; /* we must end this string */
	if(the_mode == MODE_STRING)
		current_node->car=make_string(word_buffer);
	else
		current_node->car=make_token(word_buffer);
	current_node->cdr=cons(NULL,NULL);
	current_node=current_node->cdr;
	reset();
}

void prepare_to_parse(void)
{
	reset();
	scope_stack = make_stack(128,4,0);
	push(scope_stack,lobby);
	current_col_size = 0;
	current_node = cons(NULL,NULL);
	lexical_stack = malloc(sizeof(double));
	*lexical_stack=make_cd(LIVEQ,peek(scope_stack),current_node);
	on_string = false;
	on_comment = false;	
}

void reset(void)
{	
	buffer_size = INIT_BUFFER_SIZE;
	word_buffer=realloc(word_buffer,sizeof(void*)*buffer_size);
	word_size=0;
	current_char = ' ';
}

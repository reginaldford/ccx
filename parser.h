#define INIT_BUFFER_SIZE 40

/* parser modes */
#define MODE_STRING 0
#define MODE_COMMENT 1
#define MODE_NORM 2

void finalize_quote(void);
void fix_quote(ccell* current);
void parse_file(char *file_name);
void parse_string(void);
void parse_current_char(void);
void parse_norm(void);
void check_str_end(void);
void check_comment_end(void);
void add_to_word(void);
void go_up_level(bool is_quote);
void go_down_level(void);
void check_buffer(void);
void convert(int mode);
void prepare_to_parse(void);
void reset(void);


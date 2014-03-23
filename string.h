typedef struct
{
	int tag;
	int length;
	/*content is implicitly after length*/
} string;

string* make_string(char *str);
char switch_escape(char c, bool truncating);
string* format_string(string* orig);
string* unformat_string(string* orig);
char* get_str_content(string* str);

#include "cx.h"

string* make_string(char *str)
{
	int length = strlen(str);
	string *new_string = cx_malloc(sizeof(string)+length);
	new_string->tag = TAG_STRING;
	new_string->length = length;
	memcpy(get_str_content(new_string),str,length);
	return new_string;
}

char* get_str_content(string* str)
{
	return (char*)(str+1);
}

char switch_escape(char c,bool truncating)
{
	char* escapes = "n\\qafrtv";
	char* correlating_chars = "\n\\\"\a\f\r\t\v";
	int num_escapes = 8;
	int index ;
	if(!truncating)
	{
		char* temp = escapes;
		escapes = correlating_chars;
		correlating_chars = temp;
	}
	for(index =0;index<num_escapes;index++)
		if(c == escapes[index])
			return correlating_chars[index];
	return '\0';
}


string* format_string(string* orig)
{
	char* ocd_string = get_str_content(orig);
	int ocd_str_len = orig->length;
	char* ret_string = malloc(ocd_str_len);
	int ret_str_len = ocd_str_len;
	int index=0;
	int ret_string_index=0;
	while(index < ocd_str_len)
	{
		if(ocd_string[index] == '\\' )
		{
			char the_replacement = switch_escape(ocd_string[index+1],true);
			if( the_replacement != '\0')
			{
				ret_str_len --;
				ret_string[ret_string_index] = the_replacement;
				index++;
			}
			else
				ret_string[ret_string_index] = ocd_string[index];
		}
		else
			ret_string[ret_string_index] = ocd_string[index];
		index++;
		ret_string_index++;
	}
	ret_string[ret_str_len]='\0';
	return make_string(ret_string);
}

string* unformat_string(string* orig)
{
	char* ocd_string = get_str_content(orig);
	int ocd_str_len = orig->length;
	char* new_string = malloc(ocd_str_len*2*sizeof(char));
	int new_str_len = ocd_str_len;
	int index=0;
	int new_string_index=0;
	while(index < ocd_str_len)
	{
		char c = switch_escape(ocd_string[index],false);
		if(c != '\0' )
		{
			new_str_len ++;
			new_string[new_string_index] = '\\';
			new_string[new_string_index+1]=c;
			new_string_index++;
		}
		else
			new_string[new_string_index] = ocd_string[index];
		index++;
		new_string_index++;
	}
	new_string[new_str_len]='\0';
	return make_string(new_string);
}

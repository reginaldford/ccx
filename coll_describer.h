typedef struct
{
	int tag;
	int col_tag;
	void* scope;
	ccell *content;
} cd;
cd *make_cd(int col_tag, void* scope,ccell *first_node);

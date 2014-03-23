#include "cx.h"

cd *make_cd(int col_tag,void* scope, ccell *content)
{
	cd *make_LD = cx_malloc(sizeof(cd));
	make_LD->tag=TAG_CD;
	make_LD->scope=scope;
	make_LD->col_tag=col_tag;
	make_LD->content = content;
	return make_LD;
}

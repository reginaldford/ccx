#define WAIT_PRIMITIVE(name)			\
void prim_##name(void) {			\
    push(wait_stack,make_prim(&prim_##name##_impl));	\
}						\
void prim_##name##_impl(void)

#define WAIT_PRIM_PT(name)		\
void prim_##name(void);			\
void prim_##name##_impl(void)

typedef struct {
	int tag;
	void(*block)(void);
} primitive;

primitive* make_prim (void(*pt2Func)()) ;


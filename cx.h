/* project-wide declarations */
#define TAG_CCELL 0
#define TAG_CD 1
#define TAG_STRING 2
#define TAG_INT 3
#define TAG_SYMBOL 4
#define TAG_ARRAY 5
#define TAG_PRIMITIVE 6
#define TAG_I_S 7
#define TAG_CONTEXT 8
#define TAG_NIL 9
#define TAG_GCO 10
#define TAG_BIND 11
#define TAG_STACK 12

/* fixnums! */
#define MAKE_CX_INT(x) (void*)((x)<<1|1)
#define UNTAG_CX_INT(x) ( ((int)(x)) >>1)
#define ALIGN(x)  (((x) + 3) & ~3)

/* col tags */
#define LITLIST 0
#define LIVEQ 2
#define LIST 3
#define QUOTE 4

/* standard libraries */
#include <ctype.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <setjmp.h>

/* my headers */
#include "start.h"
#include "primitive.h"
#include "cons_cell.h"
#include "coll_describer.h"
#include "environment.h"
#include "string.h"
#include "symbol.h"
#include "parser.h"
#include "tokenizer.h"
#include "evaluator.h"
#include "binding.h"
#include "array.h"
#include "context.h"
#include "inp_stub.h"
#include "math.h"
#include "repl.h"
#include "basics.h"
#include "init.h"
#include "error.h"
#include "stack.h"
#include "gc.h"
#include "memory.h"
#include "image.h"

/* conventions */
#define KB * 1024
#define MB * 1024 * 1024

/* project wide globals */
context* lobby;
stack* callstack;
stack* wait_stack;
stack* input_stack;
stack* collections;

stack* scope_stack;
context* current_scope;
ccell* symbol_table;
int symbol_table_size;
void* input;
ccell* current;

primitive* trigger;
primitive* collector;
primitive* inper;
primitive* w_killer;
primitive* prim_gc;

int universe_size;
void* universe;
int pos;
void* new_space;
int new_space_pos;

symbol* cx_true;
symbol* cx_false;
symbol* cx_null;

char* error_message;

jmp_buf boot_safepoint;
jmp_buf runtime_safepoint;

/* project-wide helpful functions */
void debug(char * message);

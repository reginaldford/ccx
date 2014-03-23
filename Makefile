MY_FILES = cx.c error.c image.c gc.c memory.c primitive.c \
cons_cell.c coll_describer.c environment.c string.c symbol.c parser.c \
tokenizer.c context.c evaluator.c binding.c start.c array.c inp_stub.c \
math.c repl.c basics.c init.c stack.c

default : $(MY_FILES)
		gcc -ldl -g -Wall -o cx $(MY_FILES)
clean :
	rm cx
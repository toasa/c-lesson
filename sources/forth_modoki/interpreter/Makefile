SRCS = token.c token_test.c stack.c stack_test.c util.c\
       test.c elem.c dict.c dict_test.c eval.c eval_test.c\
       main.c continuation.c

CFLAGS = -g

a.out: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

.PHONY: clean

clean:
	rm -rf a.out

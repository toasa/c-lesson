CC = arm-linux-gnueabi-gcc
SRCS = eval.c parser.c ps_jit.c asm.c

a.out: $(SRCS)
	$(CC) $(SRCS)
	
run: a.out
	qemu-arm -L /usr/arm-linux-gnueabi ./a.out

.PHONY: clean

clean:
	rm -rf a.out
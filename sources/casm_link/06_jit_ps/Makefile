CC = arm-linux-gnueabi-gcc
SRCS = parser.c ps_jit.c ps_jit_test.c asm.c

a.out: $(SRCS)
	$(CC) $(SRCS)
	
run: a.out
	qemu-arm -L /usr/arm-linux-gnueabi ./a.out

.PHONY: clean

clean:
	rm -rf a.out
SRCS = eval.c parser.c ps_jit.c asm.c

run: $(SRCS)
	arm-linux-gnueabi-gcc $(SRCS) && qemu-arm -L /usr/arm-linux-gnueabi ./a.out

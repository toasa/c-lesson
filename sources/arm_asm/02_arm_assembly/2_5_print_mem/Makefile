SRC=print_hex

run:
	arm-none-eabi-as ${SRC}.s -o ${SRC}.o
	arm-none-eabi-ld ${SRC}.o -Ttext 0x00010000 -o ${SRC}.elf
	arm-none-eabi-objcopy ${SRC}.elf -O binary ${SRC}.bin
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel ${SRC}.bin -serial mon:stdio

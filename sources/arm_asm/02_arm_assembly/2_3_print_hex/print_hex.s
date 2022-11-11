/*
 arm-none-eabi-as print_hex.s -o print_hex.o
 arm-none-eabi-ld print_hex.o -Ttext 0x00010000 -o print_hex.elf
 arm-none-eabi-objcopy print_hex.elf -O binary print_hex.bin
 qemu-system-arm -M versatilepb -m 128M -nographic -kernel print_hex.bin -serial mon:stdio
*/
.globl _start
_start:
    ldr r1,=0xdeadbeaf
    ldr r2,=0x101f1000
    ldr r3,=hexchar

    // Calculate number of digits
    mov r8, r1
    mov r9, #0
calc_num_of_digit:
    add r9, r9, #1
    lsr r8, r8, #4
    cmp r8, #0
    bne calc_num_of_digit

    // Print '0x'
    mov r4, #0x30
    str r4, [r2]
    mov r4, #0x78
    str r4, [r2]

print_hex:
    // Get half byte. From most significant to least.
    mov r4, r9
    sub r4, #1
    lsl r4, #2
    lsr r4, r1, r4
    and r4, r4, #0xF

    // Print hex
    add r4, r4, r3
    ldrb r5, [r4]
    str r5, [r2]

    // Shift and jump if needed
    sub r9, #1 // decrement counter
    cmp r9, #0
    bne print_hex

    // Print newline
    mov r4, #0x0A
    str r4, [r2]

end:
    b end

hexchar:
    .asciz "0123456789ABCDEF"
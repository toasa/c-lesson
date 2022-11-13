/*
 arm-none-eabi-as print_hex.s -o print_hex.o
 arm-none-eabi-ld print_hex.o -Ttext 0x00010000 -o print_hex.elf
 arm-none-eabi-objcopy print_hex.elf -O binary print_hex.bin
 qemu-system-arm -M versatilepb -m 128M -nographic -kernel print_hex.bin -serial mon:stdio
*/
.globl _start
_start:
    ldr r0,=0xdeadbeaf

//
// print_hex:
//   arg r0: target string which is printed.
//   used internal register: r2, r4, r5.
//
print_hex:
    // Set UART address.
    ldr r2, =0x101f1000

    // Print '0x'.
    mov r4, #0x30
    str r4, [r2]
    mov r4, #0x78
    str r4, [r2]

    // Set counter.
    mov r4, #8

_loop:
    // Decrement counter.
    sub r4, r4, #1

    // Get half byte.
    mov r5, r4
    lsl r5, #2
    lsr r5, r0, r5
    and r5, r5, #0xF

    // Determine alphabet ('A'-'F') or digit ('0'-'9').
    cmp r5, #0x09
    ble _digit

    // Alphabet case.
    add r5, r5, #0x37 // 'A' - 0x0a
    b _print_char

_digit:
    add r5, r5, #0x30 // '0' - 0x00

_print_char:
    str r5, [r2]

    // Continue until 4 byte integer printed as hex.
    cmp r4, #0
    bne _loop

    // Print newline.
    mov r5, #0x0A
    str r5, [r2]

end:
    b end
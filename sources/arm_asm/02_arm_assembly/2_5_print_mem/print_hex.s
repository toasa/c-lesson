.globl _start
_start:
    ldr r13, =0x08000000 // Set stack pointer
    ldr r0, =0xdeadbeaf
    bl print_hex
    ldr r0, =0x00234500
    bl print_hex
end:
    b end 

//
// print_hex:
//   arg r0: target string which is printed.
//   used internal register: r2, r4, r5.
//
print_hex:
    // prologue
    stmdb r13!, {r14}

    // Print '0x'.
    mov r5, #0x30
    bl putchar
    mov r5, #0x78
    bl putchar

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
    bl putchar

    // Continue until 4 byte integer printed as hex.
    cmp r4, #0
    bne _loop

    // Print newline.
    mov r5, #0x0A
    bl putchar

    // epilogue
    ldmia r13!, {r14}
    mov r15, r14 

//
// arg r5: one character is printed.
// used internal register: r1
//
putchar:
    ldr r1, =0x101f1000
    str r5, [r1]
    mov r15, r14 
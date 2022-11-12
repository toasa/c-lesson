.globl _start
_start:

  mov r0, r15
  bl print_hex

  mov r0, #0x68
  bl print_hex

end:
  b end

//
// Print number as hexadecimal representation.
//
// r0 : input number
// r14: caller's program counter
print_hex:

  ldr r2,=0x101f1000
  ldr r3,=hexchar

  // Calculate number of digits
  mov r8, r0
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

print_hex_main:
  // Get half byte. From most significant to least.
  mov r4, r9
  sub r4, #1
  lsl r4, #2
  lsr r4, r0, r4
  and r4, r4, #0xF

  // Print hex
  add r4, r4, r3
  ldrb r5, [r4]
  str r5, [r2]

  // Shift and jump if needed
  sub r9, #1 // decrement counter
  cmp r9, #0
  bne print_hex_main

  // Print newline
  mov r4, #0x0A
  str r4, [r2]

  // Return to caller code
  mov r15, r14

hexchar:
  .asciz "0123456789ABCDEF"
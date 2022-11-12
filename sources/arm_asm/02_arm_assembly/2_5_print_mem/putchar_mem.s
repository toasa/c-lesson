.globl _start
_start:
  ldr r13,=0x08000000 // Set stack pointer
  ldr r0,=msg1
  bl print
  ldr r0,=msg2
  bl print
end:
  b end 

//
// arg r3: one character is printed.
// used internal register: r1
//
putchar:
  ldr r1,=0x101f1000
  str r3, [r1]
  mov r15, r14 

//
// arg r0: address of the string which is printed.
// used internal register: r0, r3
//
print:
  // prologue
  stmdb r13!, {r14}

  ldrb r3,[r0]
_loop:  
  bl putchar

  add r0, r0, #1
  ldrb r3, [r0]
  cmp r3, #0
  bne _loop

  // epilogue
  ldmia r13!, {r14}
  mov r15, r14 

msg1: .asciz "First text.\n"
msg2: .asciz "Second text!\n"
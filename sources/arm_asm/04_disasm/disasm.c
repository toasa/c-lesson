#include "disasm.h"

enum opcode {
    OP_MOV = 0b1101,
};

//
// See the following ARM7DI data sheet for more detail:
//
// https://developer.arm.com/docs/ddi0027/latest/arm7di-data-sheet
//
static int print_asm(int word) {
    int imm = word & 0xFF;
    int dst_reg = (word >> 12) & 0x0F;
    int opcode = (word >> 21) & 0x0F;

    if (opcode == OP_MOV) {
        cl_printf("mov r%d, #%#x\n", dst_reg, imm);
        return 0;
    }

    return -1;
}

#define ARR_SIZE(a) (int)(sizeof(a) / sizeof(a[0]))

static void test_disasm_mov(void) {
    int words[] = {
        0xE3A01068,
        0xE3A01065,
        0xE3A0200D,
        0xE3A0200A,
    };

    char *expecteds[] = {
        "mov r1, #0x68\n",
        "mov r1, #0x65\n",
        "mov r2, #0xd\n",
        "mov r2, #0xa\n",
    };

    cl_enable_buffer_mode();

    for (int i = 0; i < ARR_SIZE(words); i++) {
        int word = words[i];
        char *expected = expecteds[i];

        assert_int_eq(print_asm(word), 0);

        char *actual = cl_get_result(i);

        assert_str_eq(expected, actual);
    }

    cl_clear_output();
}

void test_disasm(void) { test_disasm_mov(); }
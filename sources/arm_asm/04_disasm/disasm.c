#include "disasm.h"

enum opcode {
    OP_MOV = 0b1101,
};

static int print_asm_mov(int word);
static int print_asm_branch(int word);
static int print_asm_ldr_str(int word);

//
// See the following ARM7DI data sheet for more detail:
//
// https://developer.arm.com/docs/ddi0027/latest/arm7di-data-sheet
//
static int print_asm(int word) {
    int id = (word >> 25) & 0b111;
    switch (id) {
    case 0b001:
        return print_asm_mov(word);
        break;
    case 0b101:
        return print_asm_branch(word);
        break;
    case 0b010:
        return print_asm_ldr_str(word);
        break;
    default:
        return -1;
    }
}

static int print_asm_mov(int word) {
    int imm = word & 0xFF;
    int dst_reg = (word >> 12) & 0x0F;
    int opcode = (word >> 21) & 0x0F;

    if (opcode == OP_MOV) {
        cl_printf("mov r%d, #%#x\n", dst_reg, imm);
        return 0;
    }

    return -1;
}

static int print_asm_branch(int word) {
    int offset = word | 0xFF000000;

    cl_printf("b [r15, #%d]\n", offset);
    return 0;
}

static int print_asm_ldr_str(int word) {
    bool is_load = ((word >> 20) & 0x01) == 1;

    int src_dst_reg = (word >> 12) & 0x0F;
    int base_reg = (word >> 16) & 0x0F;

    int offset = word & 0x0FFF;

    if (is_load)
        cl_printf("ldr r%d, [r15, #%d]\n", src_dst_reg, offset);
    else
        cl_printf("str r%d, [r%d]\n", src_dst_reg, base_reg);

    return 0;
}

#define ARR_SIZE(a) (int)(sizeof(a) / sizeof(a[0]))

static void _test_disasm(void) {
    int words[] = {
        // mov
        0xE3A01068,
        0xE3A01065,
        0xE3A0200D,
        0xE3A0200A,

        // b
        0xEAFFFFFE,

        // ldr
        0xE59F0038,

        // str
        0xE5801000,
        0xE5802000,
    };

    char *expecteds[] = {
        "mov r1, #0x68\n", "mov r1, #0x65\n", "mov r2, #0xd\n",
        "mov r2, #0xa\n",  "b [r15, #-2]\n",  "ldr r0, [r15, #56]\n",
        "str r1, [r0]\n",  "str r2, [r0]\n",
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

void test_disasm(void) { _test_disasm(); }
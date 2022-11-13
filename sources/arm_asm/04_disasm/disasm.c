#include "disasm.h"

static int print_asm(int word) {
    if (word == 0xE3A01068) {
        cl_printf("mov r1, #0x68\n");
        return 0;
    }

    return -1;
}

#define ARR_SIZE(a) (int)(sizeof(a) / sizeof(a[0]))

static void test_disasm_mov(void) {
    int words[] = {
        0xE3A01068,
        0xE3A01068,
    };

    char *expecteds[] = {
        "mov r1, #0x68\n",
        "mov r1, #0x68\n",
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
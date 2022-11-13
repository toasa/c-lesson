#include "disasm.h"

int print_asm(int word) {
    if (word == 0xE3A01068) {
        cl_printf("mov r1, #0x68\n");
        return 0;
    }

    return -1;
}

void test_disasm_mov(void) {
    int word = 0xE3A01068;
    cl_enable_buffer_mode();

    int res = print_asm(word);
    assert(res == 0);

    char *actual = cl_get_result(0);

    assert_str_eq("mov r1, #0x68\n", actual);

    cl_clear_output();
}

int main() { test_disasm_mov(); }
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "asm.h"
#include "parser.h"
#include "ps_jit.h"
#include "ps_jit_test.h"

static int *binary_buf = NULL;

static int *allocate_executable_buf(int size) {
    return (int *)mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

static void ensure_jit_buf() {
    if (binary_buf == NULL) {
        binary_buf = allocate_executable_buf(1024);
    }
}

int *jit_script(char *input) {
    ensure_jit_buf();

    struct Substr remain = {input, strlen(input)};
    int pos = 0;

    while (!is_end(&remain)) {
        skip_space(&remain);
        if (is_number(remain.ptr)) {
            int8_t imm = (int8_t)parse_number(remain.ptr);
            binary_buf[pos++] = asm_mov_imm(R2, imm);
            binary_buf[pos++] = asm_stfmd(R2);

            skip_token(&remain);
            continue;
        } else if (is_register(remain.ptr)) {
            int reg = remain.ptr[1] == '1' ? R1 : R0;
            binary_buf[pos++] = asm_stfmd(reg);

            skip_token(&remain);
            continue;
        } else {
            // Must be op.
            int op = parse_word(&remain);
            skip_token(&remain);

            // R2 is operand of left hand side and R3 is right.
            binary_buf[pos++] = asm_ldmia(R3);
            binary_buf[pos++] = asm_ldmia(R2);
            switch (op) {
            case OP_ADD:
                binary_buf[pos++] = asm_add(R2, R2, R3);
                break;
            case OP_SUB:
                binary_buf[pos++] = asm_sub(R2, R2, R3);
                break;
            case OP_MUL:
                binary_buf[pos++] = asm_mul(R2, R2, R3);
                break;
            }

            binary_buf[pos++] = asm_stfmd(R2);
        }
    }

    // Pop the calculation result and store it into r0 as return value.
    binary_buf[pos++] = asm_ldmia(R2);
    binary_buf[pos++] = asm_mov(R0, R2);

    // Function epilogue.
    binary_buf[pos++] = asm_mov(PC, LR);

    return binary_buf;
}

int main() {
    run_unit_tests();

    return 0;
}

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "asm.h"
#include "parser.h"
#include "ps_jit.h"
#include "ps_jit_test.h"

#define BUF_SIZE 1024
static int *_binary_buf = NULL;

struct Emitter {
    int *binary_buf;
    int pos;
};

static int *allocate_executable_buf(int size) {
    return (int *)mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

static struct Emitter *new_emitter(void) {
    struct Emitter *e = calloc(1, sizeof(struct Emitter));

    // We create buffer only once.
    if (_binary_buf == NULL)
        _binary_buf = allocate_executable_buf(BUF_SIZE);

    e->binary_buf = _binary_buf;
    e->pos = 0;

    return e;
}

static void emit(struct Emitter *e, int word) {
    e->binary_buf[e->pos++] = word;
}

static int *emitter_get_buf(struct Emitter *e) { return e->binary_buf; }

int *jit_script(char *input) {
    struct Emitter *e = new_emitter();
    struct Substr remain = {input, strlen(input)};

    while (!is_end(&remain)) {
        skip_space(&remain);
        if (is_number(remain.ptr)) {
            int imm = parse_number(remain.ptr);
            emit(e, asm_mov_imm(R2, imm));
            emit(e, asm_stfmd(R2));

            skip_token(&remain);
            continue;
        } else if (is_register(remain.ptr)) {
            int reg = remain.ptr[1] == '1' ? R1 : R0;
            emit(e, asm_stfmd(reg));

            skip_token(&remain);
            continue;
        } else {
            // Must be op.
            int op = parse_word(&remain);
            skip_token(&remain);

            // R2 is operand of left hand side and R3 is right.
            emit(e, asm_ldmia(R3));
            emit(e, asm_ldmia(R2));

            switch (op) {
            case OP_ADD:
                emit(e, asm_add(R2, R2, R3));
                break;
            case OP_SUB:
                emit(e, asm_sub(R2, R2, R3));
                break;
            case OP_MUL:
                emit(e, asm_mul(R2, R2, R3));
                break;
            }

            emit(e, asm_stfmd(R2));
        }
    }

    // Pop the calculation result and store it into r0 as return value.
    emit(e, asm_ldmia(R2));
    emit(e, asm_mov(R0, R2));

    // Function epilogue.
    emit(e, asm_mov(PC, LR));

    return emitter_get_buf(e);
}

int main() {
    run_unit_tests();

    return 0;
}

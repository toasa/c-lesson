#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "asm.h"
#include "parser.h"
#include "test_util.h"

extern int eval(int r0, int r1, char *str);

int *binary_buf = NULL;

int *allocate_executable_buf(int size) {
    return (int *)mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void ensure_jit_buf() {
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

            if (op != OP_ADD) {
                fprintf(stderr, "Unknown token");
                exit(1);
            }

            // R2 is operand of left hand side and R3 is right.
            binary_buf[pos++] = asm_ldmia(R3);
            binary_buf[pos++] = asm_ldmia(R2);
            binary_buf[pos++] = asm_add(R2, R2, R3);
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

#define ARR_SIZE(a) (int)(sizeof(a) / sizeof(a[0]))

struct JITContext {
    char *input;
    int r0;
    int r1;
};

static int run_jit_script(struct JITContext *ctx) {
    int (*funcvar)(int, int) = (int (*)(int, int))jit_script(ctx->input);
    return funcvar(ctx->r0, ctx->r1);
}

static void test_single_int(void) {
    struct JITContext ctx = {
        .input = "123",
        .r0 = 10,
        .r1 = 20,
    };

    int res = run_jit_script(&ctx);
    assert_int_eq(res, 123);
}

static void test_register(void) {
    struct JITContext ctxs[] = {
        {
            .r0 = 10,
            .r1 = 20,
            .input = "r0",
        },
        {
            .r0 = 10,
            .r1 = 20,
            .input = "r1",
        },
    };

    int expecteds[] = {
        10,
        20,
    };

    for (int i = 0; i < ARR_SIZE(ctxs); i++) {
        int actual = run_jit_script(&ctxs[i]);
        int expected = expecteds[i];

        assert_int_eq(actual, expected);
    }
}

static void test_multi_int_and_reg(void) {
    struct JITContext ctxs[] = {
        {
            .r0 = 11,
            .r1 = 22,
            .input = "10 20 30",
        },
        {
            .r0 = 11,
            .r1 = 22,
            .input = "10 20 r0",
        },
        {
            .r0 = 11,
            .r1 = 22,
            .input = "10 20 r1",
        },
    };

    int expecteds[] = {
        30,
        11,
        22,
    };

    for (int i = 0; i < ARR_SIZE(ctxs); i++) {
        int actual = run_jit_script(&ctxs[i]);
        int expected = expecteds[i];

        assert_int_eq(actual, expected);
    }
}

static void test_add(void) {
    struct JITContext ctx = {
        .input = "11 22 add",
        .r0 = 10,
        .r1 = 20,
    };

    int res = run_jit_script(&ctx);
    assert_int_eq(res, 33);
}

static void run_unit_tests() {
    test_single_int();
    test_register();
    test_multi_int_and_reg();
    test_add();

    printf("all test done\n");
}

int main() {
    run_unit_tests();

    int res = eval(1, 5, "3 7 add r1 sub 4 mul");
    printf("res=%d\n", res);

    // /*
    //  TODO: Make below test pass.
    // */
    // funcvar = (int (*)(int, int))jit_script("3 7 add r1 sub 4 mul");

    // res = funcvar(1, 5);
    // assert_int_eq(20, res);

    // res = funcvar(1, 4);
    // assert_int_eq(24, res);

    return 0;
}

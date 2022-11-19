#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "parser.h"
#include "test_util.h"

extern int eval(int r0, int r1, char *str);

/*
JIT
*/
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

    int8_t imm = (int8_t)parse_number(input);

    binary_buf[0] = 0xE3A02000 | imm; // mov r2, imm
    binary_buf[1] = 0xE92D0004;       // stmfd sp!, {r2}
    binary_buf[2] = 0xE8BD0004;       // ldmia sp!, {r2}
    binary_buf[3] = 0xE1A00002;       // mov r0, r2
    binary_buf[4] = 0xE1A0F00E;       // mov r15, r14

    return binary_buf;
}

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

static void run_unit_tests() {
    test_single_int();

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

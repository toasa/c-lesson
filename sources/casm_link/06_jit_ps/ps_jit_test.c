#include <stdio.h>

#include "ps_jit.h"
#include "test_util.h"

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
            .input = "r0",
            .r0 = 10,
            .r1 = 20,
        },
        {
            .input = "r1",
            .r0 = 10,
            .r1 = 20,
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
            .input = "10 20 30",
            .r0 = 11,
            .r1 = 22,
        },
        {
            .input = "10 20 r0",
            .r0 = 11,
            .r1 = 22,
        },
        {
            .input = "10 20 r1",
            .r0 = 11,
            .r1 = 22,
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

static void test_sub_mul(void) {
    struct JITContext ctxs[] = {
        {
            .input = "r0 r1 sub",
            .r0 = 6,
            .r1 = 4,
        },
        {
            .input = "r0 r1 mul",
            .r0 = 6,
            .r1 = 4,
        },
    };

    int expecteds[] = {
        2,
        24,
    };

    for (int i = 0; i < ARR_SIZE(ctxs); i++) {
        int actual = run_jit_script(&ctxs[i]);
        int expected = expecteds[i];

        assert_int_eq(actual, expected);
    }
}

static void test_add_sub_mul(void) {
    char *input = "3 7 add r1 sub 4 mul";

    struct JITContext ctxs[] = {
        {
            .input = input,
            .r0 = 1,
            .r1 = 5,
        },
        {
            .input = input,
            .r0 = 1,
            .r1 = 4,
        },
    };

    int expecteds[] = {
        20,
        24,
    };

    for (int i = 0; i < ARR_SIZE(ctxs); i++) {
        int actual = run_jit_script(&ctxs[i]);
        int expected = expecteds[i];

        assert_int_eq(actual, expected);
    }
}

void run_unit_tests(void) {
    test_single_int();
    test_register();
    test_multi_int_and_reg();
    test_add();
    test_sub_mul();
    test_add_sub_mul();

    printf("all test done\n");
}
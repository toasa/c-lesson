#include <assert.h>

#include "clesson.h"
#include "parser.h"
#include "stack.h"
#include "token.h"
#include "util.h"

void eval(void) {
    int ch = EOF;
    struct Token t = {UNKNOWN, {0}};

    do {
        ch = parse_one(ch, &t);

        switch (t.ltype) {
        case NUMBER:
            stack_push(new_num_token(t.u.number));
            break;
        case EXECUTABLE_NAME: {
            struct Token *rhs = stack_pop();
            struct Token *lhs = stack_pop();
            stack_push(new_num_token(lhs->u.number + rhs->u.number));
        }
        default:
            break;
        }
    } while (ch != EOF);

    if (!stack_is_empty())
        stack_print_all();
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop()->u.number;

    assert(expect == actual);
}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    cl_getc_set_src(input);

    eval();

    int actual1 = stack_pop()->u.number;
    int actual2 = stack_pop()->u.number;

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}

static void test_eval_num_add() {
    char *input = "1 2 add";
    int expect = 3;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_num_add_one_to_nine() {
    char *input = "1 2 3 add add 4 5 6 7 8 9 add add add add add add";
    int expect = 45;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}
int main() {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_num_add_one_to_nine();

    return 0;
}

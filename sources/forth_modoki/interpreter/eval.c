#include "clesson.h"

static void eval_exe_name(const char *name) {
    struct Element e = {0};

    if (streq(name, "add")) {
        struct Element *rhs = stack_pop();
        struct Element *lhs = stack_pop();
        stack_push(new_num_element(lhs->u.number + rhs->u.number));
    } else if (streq(name, "def")) {
        struct Element *val = stack_pop();
        struct Element *key = stack_pop();
        dict_put(key->u.name, val);
    } else if (dict_get(name, &e)) {
        stack_push(copy_element(&e));
    }
}

void eval(void) {
    int ch = EOF;
    struct Token t = {UNKNOWN, {0}};

    do {
        ch = parse_one(ch, &t);

        switch (t.ltype) {
        case NUMBER:
            stack_push(new_num_element(t.u.number));
            break;
        case EXECUTABLE_NAME:
            eval_exe_name(t.u.name);
            break;
        case LITERAL_NAME:
            stack_push(new_lit_name_element(t.u.name));
            break;
        default:
            break;
        }
    } while (ch != EOF);

    // if (!stack_is_empty())
    //     stack_print_all();
}

static void test_eval_num_one(void) {
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop()->u.number;

    assert(expect == actual);
}

static void test_eval_num_two(void) {
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

static void test_eval_num_add(void) {
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

static void test_eval_variable(void) {
    char *input = "/foo 11 def foo 20 add";
    int expect = 31;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

void test_eval(void) {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_num_add_one_to_nine();
    test_eval_variable();

    printf("%s: OK\n", __func__);
}
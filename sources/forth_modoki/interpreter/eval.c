#include "clesson.h"

static void add_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number + rhs->u.number));
}

static void sub_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number - rhs->u.number));
}

static void mul_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number * rhs->u.number));
}

static void div_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number / rhs->u.number));
}

static void def(void) {
    struct Element *val = stack_pop();
    struct Element *key = stack_pop();
    dict_put(key->u.name, val);
}

static void register_primitives(void) {
    dict_put("add", new_cfunc_element(add_op));
    dict_put("sub", new_cfunc_element(sub_op));
    dict_put("mul", new_cfunc_element(mul_op));
    dict_put("div", new_cfunc_element(div_op));
    dict_put("def", new_cfunc_element(def));
}

static void eval_exe_name(const char *name) {
    struct Element *e = dict_get(name);
    switch (e->etype) {
    case ELEM_C_FUNC:
        e->u.cfunc();
        break;
    default: {
        if (e == NULL) {
            fprintf(stderr, "Unbind variable: %s\n", name);
            abort();
        }
        stack_push(e);
        break;
    }
    }
}

void eval(struct Token *tokens[]) {
    for (int i = 0; tokens[i]->ltype != END_OF_FILE; i++) {
        struct Token *t = tokens[i];
        switch (t->ltype) {
        case NUMBER:
            stack_push(new_num_element(t->u.number));
            break;
        case EXECUTABLE_NAME:
            eval_exe_name(t->u.name);
            break;
        case LITERAL_NAME:
            stack_push(new_lit_name_element(t->u.name));
            break;
        default:
            break;
        }
    }

    // if (!stack_is_empty())
    //     stack_print_all();
}

static void test_eval_num_one(void) {
    char *input = "123";
    int expect = 123;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;

    assert_int_eq(expect, actual);
}

static void test_eval_num_two(void) {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual1 = stack_pop()->u.number;
    int actual2 = stack_pop()->u.number;

    assert_int_eq(expect1, actual1);
    assert_int_eq(expect2, actual2);
}

static void test_eval_num_add(void) {
    char *input = "1 2 add";
    int expect = 3;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_num_sub(void) {
    char *input = "5 3 sub";
    int expect = 2;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_num_mul(void) {
    char *input = "5 3 mul";
    int expect = 15;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_num_div(void) {
    char *input = "90 5 div";
    int expect = 18;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_num_add_one_to_nine() {
    char *input = "1 2 3 add add 4 5 6 7 8 9 add add add add add add";
    int expect = 45;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_variable(void) {
    char *input = "/foo 11 def foo 20 add";
    int expect = 31;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

void test_eval(void) {
    register_primitives();

    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_num_sub();
    test_eval_num_mul();
    test_eval_num_div();
    test_eval_num_add_one_to_nine();
    test_eval_variable();

    printf("%s: OK\n", __func__);
}
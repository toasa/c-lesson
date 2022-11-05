#include "clesson.h"

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

static void test_eval_num_mod(void) {
    char *input = "11 7 mod";
    int expect = 4;

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

static void test_eval_eq(void) {
    char *input = "1 2 eq";
    int expect = 0;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_neq(void) {
    char *input = "1 2 neq";
    int expect = 1;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_gt(void) {
    char *input = "1 2 gt";
    int expect = 0;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_lt(void) {
    char *input = "1 2 lt";
    int expect = 1;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_pop(void) {
    char *input = "1 2 pop";
    int expect = 1;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_exch(void) {
    char *input = "1 2 exch";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_pop()->u.number == 1);
    assert(stack_pop()->u.number == 2);
}

static void test_eval_dup(void) {
    char *input = "1 2 dup";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_pop()->u.number == 2);
    assert(stack_pop()->u.number == 2);
}

static void test_eval_index0(void) {
    char *input = "1 2 3 4 5 0 index";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_pop()->u.number == 5);
    assert(stack_pop()->u.number == 5);
    assert(stack_pop()->u.number == 4);
}

static void test_eval_index2(void) {
    char *input = "1 2 3 4 5 2 index";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_pop()->u.number == 3);
    assert(stack_pop()->u.number == 5);
    assert(stack_pop()->u.number == 4);
    assert(stack_pop()->u.number == 3);
}

static void test_eval_roll(void) {
    char *input = "1 2 3 4 5 6 7 4 3 roll";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_pop()->u.number == 4);
    assert(stack_pop()->u.number == 7);
    assert(stack_pop()->u.number == 6);
    assert(stack_pop()->u.number == 5);
    assert(stack_pop()->u.number == 3);
}

static void test_eval_variable(void) {
    char *input = "/foo 11 def foo 20 add";
    int expect = 31;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_exec_array_one_num(void) {
    char *input = "{11}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->ty == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 1);
    assert(ea->elem[0]->u.number == 11);
}

static void test_eval_exec_array_one_lit_name(void) {
    char *input = "{/abc}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->ty == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 1);
    assert(ea->elem[0]->ty == ELEM_LITERAL_NAME);
    assert_str_eq(ea->elem[0]->u.name, "abc");
}

static void test_eval_exec_array_one_exec_name(void) {
    char *input = "{abc}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->ty == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 1);
    assert(ea->elem[0]->ty == ELEM_EXECUTABLE_NAME);
    assert_str_eq(ea->elem[0]->u.name, "abc");
}

static void test_eval_exec_array_multi_nums(void) {
    char *input = "{11 22}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->ty == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 2);
    assert(ea->elem[0]->u.number == 11);
    assert(ea->elem[1]->u.number == 22);
}

static void test_eval_exec_array_exec(void) {
    char *input = "{11 22} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_pop()->u.number == 22);
    assert(stack_pop()->u.number == 11);
}

static void test_eval_multi_exec_arrays(void) {
    char *input = "{11} {22}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e1 = stack_pop();
    struct Element *e2 = stack_pop();
    assert(e1->ty == ELEM_EXECUTABLE_ARRAY);
    assert(e2->ty == ELEM_EXECUTABLE_ARRAY);

    assert(e1->u.byte_code->len == 1);
    assert(e1->u.byte_code->elem[0]->u.number == 22);
    assert(e2->u.byte_code->len == 1);
    assert(e2->u.byte_code->elem[0]->u.number == 11);
}

static void test_eval_nested_exec_arrays(void) {
    char *input = "{11 {22} 33}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->ty == ELEM_EXECUTABLE_ARRAY);
    assert(e->u.byte_code->len == 3);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->elem[0]->u.number == 11);
    assert(ea->elem[1]->ty == ELEM_EXECUTABLE_ARRAY);
    assert(ea->elem[1]->u.byte_code->elem[0]->u.number == 22);
    assert(ea->elem[2]->u.number == 33);
}

static void test_eval_nested_exec_arrays_exec(void) {
    char *input = "{11 {22} 33} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_pop()->u.number == 33);

    struct Element *e = stack_pop();
    assert(e->ty == ELEM_EXECUTABLE_ARRAY);
    assert(e->u.byte_code->len == 1);
    assert(e->u.byte_code->elem[0]->u.number == 22);

    assert(stack_pop()->u.number == 11);
}

static void test_eval_exec_array_def_and_eval(void) {
    char *input = "/plusone { 1 add } def 100 plusone";
    int expect = 101;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_exec_array_nested_def_following(void) {
    char *input = "/f { {1 3 add} exec 3} def f";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(3 == stack_pop()->u.number);
    assert(4 == stack_pop()->u.number);
}

static void test_eval_exec_array_nested_def_and_eval(void) {
    char *input = "/ZZ {6} def /YY {4 ZZ 5} def /XX {1 2 YY 3} def XX mul add "
                  "mul add mul";
    int expect = 1 * (2 + (4 * (6 + (5 * 3))));

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_exec_array_imcomplete(void) {
    char *input = "/a { {345} ifelse} def 1 {123} a";
    int expect = 123;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_exec_op(void) {
    char *input = "{10 20 add} exec";
    int expect = 10 + 20;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_exec_op_nested(void) {
    char *input = "{{10 20 add} exec 30 add } exec";
    int expect = 10 + 20 + 30;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_if_op_true(void) {
    char *input = "1 {10 20 add} if";
    int expect = 10 + 20;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_if_op_true_nested(void) {
    char *input = "{1 {10 20 add} if} exec";
    int expect = 10 + 20;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
}

static void test_eval_if_op_false(void) {
    char *input = "0 {10 20 add} if";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_is_empty());
}

static void test_eval_if_op_false_nested(void) {
    char *input = "{0 {10 20 add} if} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(stack_is_empty());
}

static void test_eval_if_op_nested_following(void) {
    char *input = "/b { 1 { 3 } if 2 } def /a {b 1} def a";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(1 == stack_pop()->u.number);
    assert(2 == stack_pop()->u.number);
    assert(3 == stack_pop()->u.number);
}

static void test_eval_if_op_nested_following_nested(void) {
    char *input = "{/b { 1 { 3 } if 2 } def /a {b 1} def a} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(1 == stack_pop()->u.number);
    assert(2 == stack_pop()->u.number);
    assert(3 == stack_pop()->u.number);
}

static void test_eval_ifelse_op_then(void) {
    char *input = "10 1 {1 add} {2 add} ifelse";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(11 == stack_pop()->u.number);
}

static void test_eval_ifelse_op_then_nested(void) {
    char *input = "{10 1 {1 add} {2 add} ifelse} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(11 == stack_pop()->u.number);
}

static void test_eval_ifelse_op_else(void) {
    char *input = "10 0 {1 add} {2 add} ifelse";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(12 == stack_pop()->u.number);
}

static void test_eval_ifelse_op_else_nested(void) {
    char *input = "{10 0 {1 add} {2 add} ifelse} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(12 == stack_pop()->u.number);
}

static void test_eval_ifelse_op_following(void) {
    char *input = "1 {2} {3} ifelse 4";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(4 == stack_pop()->u.number);
    assert(2 == stack_pop()->u.number);
}

static void test_eval_repeat_op(void) {
    char *input = "1 10 {2 mul} repeat";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(1024 == stack_pop()->u.number);
}

static void test_eval_repeat_op_nested(void) {
    char *input = "{1 10 {2 mul} repeat} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(1024 == stack_pop()->u.number);
}

static void test_eval_repeat_op_following(void) {
    char *input = "1 10 {2 mul} repeat 111";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(111 == stack_pop()->u.number);
    assert(1024 == stack_pop()->u.number);
}

static void test_eval_while_op(void) {
    char *input = "/factorial { dup {dup 1 gt} { 1 sub exch 1 index mul exch } "
                  "while pop } def 6 factorial";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(720 == stack_pop()->u.number);
}

static void test_eval_while_op_nested(void) {
    char *input =
        "{/factorial { dup {dup 1 gt} { 1 sub exch 1 index mul exch } "
        "while pop } def 6 factorial} exec";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(720 == stack_pop()->u.number);
}

static void test_eval_while_op_following(void) {
    char *input = "/factorial { dup {dup 1 gt} { 1 sub exch 1 index mul exch } "
                  "while pop } def 6 factorial 1024";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    assert(1024 == stack_pop()->u.number);
    assert(720 == stack_pop()->u.number);
}

void test_eval(void) {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_num_sub();
    test_eval_num_mul();
    test_eval_num_div();
    test_eval_num_mod();
    test_eval_num_add_one_to_nine();
    test_eval_variable();

    test_eval_eq();
    test_eval_neq();
    test_eval_gt();
    test_eval_lt();

    test_eval_pop();
    test_eval_exch();
    test_eval_dup();
    test_eval_index0();
    test_eval_index2();
    test_eval_roll();

    test_eval_exec_array_one_num();
    test_eval_exec_array_one_lit_name();
    test_eval_exec_array_one_exec_name();
    test_eval_exec_array_multi_nums();
    test_eval_multi_exec_arrays();
    test_eval_nested_exec_arrays();
    test_eval_exec_array_exec();
    test_eval_nested_exec_arrays_exec();
    test_eval_exec_array_def_and_eval();
    test_eval_exec_array_nested_def_following();
    test_eval_exec_array_nested_def_and_eval();
    test_eval_exec_array_imcomplete();

    test_eval_exec_op();
    test_eval_exec_op_nested();
    test_eval_if_op_true();
    test_eval_if_op_true_nested();
    test_eval_if_op_false();
    test_eval_if_op_false_nested();
    test_eval_if_op_nested_following();
    test_eval_if_op_nested_following_nested();
    test_eval_ifelse_op_then();
    test_eval_ifelse_op_then_nested();
    test_eval_ifelse_op_else();
    test_eval_ifelse_op_else_nested();
    test_eval_ifelse_op_following();
    test_eval_repeat_op();
    test_eval_repeat_op_nested();
    test_eval_repeat_op_following();
    test_eval_while_op();
    test_eval_while_op_nested();
    test_eval_while_op_following();

    printf("%s: OK\n", __func__);
}
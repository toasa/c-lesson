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

static struct Token **_tokens;
static int pos = 0;

static void _init(struct Token **tokens) {
    _tokens = tokens;
    pos = 0;
}

static struct Element *compile_exec_array(void) {
    // Default capacity, expand when needed.
    int cap = 10;
    struct Element **elems = calloc(1, sizeof(struct Element) * cap);

    int len = 0;
    for (; _tokens[pos]->ltype != END_OF_FILE; pos++) {
        struct Token *t = _tokens[pos];

        switch (t->ltype) {
        case NUMBER:
            elems[len++] = new_num_element(t->u.number);
            break;
        case EXECUTABLE_NAME:
            elems[len++] = new_exec_name_element(t->u.name);
            break;
        case LITERAL_NAME:
            elems[len++] = new_lit_name_element(t->u.name);
            break;
        case OPEN_CURLY:
            pos++;
            elems[len++] = compile_exec_array();
            break;
        case CLOSE_CURLY:
            goto succuess;
        default:
            break;
        }

        if (len >= cap) {
            cap *= 2;
            elems = realloc(elems, sizeof(struct Element) * cap);
        }
    }

    // Unreachable point
    abort();

succuess:

    // Shrink the `elems` size to `len`.
    elems = realloc(elems, sizeof(struct Element) * len);

    struct ElementArray *ea = calloc(1, sizeof(struct ElementArray));
    ea->len = len;
    ea->elem = elems;

    return new_exec_array_element(ea);
}

static void eval_elem(struct Element *e);
static void eval_exec_name(const char *name);
static void eval_exec_array(struct ElementArray *elems);

static void eval_exec_name(const char *name) {
    struct Element *e = dict_get(name);
    if (e == NULL) {
        fprintf(stderr, "Unbind variable: %s\n", name);
        abort();
    }

    eval_elem(e);
}

static void eval_elem(struct Element *e) {
    switch (e->etype) {
    case ELEM_NUMBER:
    case ELEM_LITERAL_NAME:
        stack_push(e);
        break;
    case ELEM_EXECUTABLE_NAME:
        eval_exec_name(e->u.name);
        break;
    case ELEM_C_FUNC:
        e->u.cfunc();
        break;
    case ELEM_EXECUTABLE_ARRAY:
        eval_exec_array(e->u.byte_code);
        break;
    default:
        abort();
    }
}

static void eval_exec_array(struct ElementArray *elems) {
    for (int i = 0; i < elems->len; i++)
        eval_elem(elems->elem[i]);
}

void eval(struct Token *tokens[]) {
    _init(tokens);

    for (; _tokens[pos]->ltype != END_OF_FILE; pos++) {
        struct Token *t = _tokens[pos];
        switch (t->ltype) {
        case NUMBER:
            stack_push(new_num_element(t->u.number));
            break;
        case EXECUTABLE_NAME:
            eval_exec_name(t->u.name);
            break;
        case LITERAL_NAME:
            stack_push(new_lit_name_element(t->u.name));
            break;
        case OPEN_CURLY:
            pos++;
            stack_push(compile_exec_array());
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

static void test_eval_exec_array_one_num(void) {
    char *input = "{11}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->etype == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 1);
    assert(ea->elem[0]->u.number == 11);
}

static void test_eval_exec_array_one_lit_name(void) {
    char *input = "{/abc}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->etype == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 1);
    assert(ea->elem[0]->etype == ELEM_LITERAL_NAME);
    assert_str_eq(ea->elem[0]->u.name, "abc");
}

static void test_eval_exec_array_one_exec_name(void) {
    char *input = "{abc}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->etype == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 1);
    assert(ea->elem[0]->etype == ELEM_EXECUTABLE_NAME);
    assert_str_eq(ea->elem[0]->u.name, "abc");
}

static void test_eval_exec_array_multi_nums(void) {
    char *input = "{11 22}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e = stack_pop();
    assert(e->etype == ELEM_EXECUTABLE_ARRAY);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->len == 2);
    assert(ea->elem[0]->u.number == 11);
    assert(ea->elem[1]->u.number == 22);
}

static void test_eval_multi_exec_arrays(void) {
    char *input = "{11} {22}";

    struct Token **tokens = tokenize(input);
    eval(tokens);

    struct Element *e1 = stack_pop();
    struct Element *e2 = stack_pop();
    assert(e1->etype == ELEM_EXECUTABLE_ARRAY);
    assert(e2->etype == ELEM_EXECUTABLE_ARRAY);

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
    assert(e->etype == ELEM_EXECUTABLE_ARRAY);
    assert(e->u.byte_code->len == 3);

    struct ElementArray *ea = e->u.byte_code;
    assert(ea->elem[0]->u.number == 11);
    assert(ea->elem[1]->etype == ELEM_EXECUTABLE_ARRAY);
    assert(ea->elem[1]->u.byte_code->elem[0]->u.number == 22);
    assert(ea->elem[2]->u.number == 33);
}

static void test_eval_exec_array_def_and_eval(void) {
    char *input = "/plusone { 1 add } def 100 plusone";
    int expect = 101;

    struct Token **tokens = tokenize(input);
    eval(tokens);

    int actual = stack_pop()->u.number;
    assert(expect == actual);
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
    test_eval_exec_array_one_num();
    test_eval_exec_array_one_lit_name();
    test_eval_exec_array_one_exec_name();
    test_eval_exec_array_multi_nums();
    test_eval_multi_exec_arrays();
    test_eval_nested_exec_arrays();
    test_eval_exec_array_def_and_eval();
    test_eval_exec_array_nested_def_and_eval();

    printf("%s: OK\n", __func__);
}
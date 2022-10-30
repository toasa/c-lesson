#include "clesson.h"

static void test_tokenize_empty_string(void) {
    char *input = "";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ltype, END_OF_FILE);
}

static void test_tokenize_one_number(void) {
    char *input = "123";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ltype, NUMBER);
    assert_int_eq(tokens[0]->u.number, 123);
}

static void test_tokenize_one_executable_name(void) {
    char *input = "add";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ltype, EXECUTABLE_NAME);
    assert_str_eq(tokens[0]->u.name, "add");
}

static void test_tokenize_one_literal_name(void) {
    char *input = "/add";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ltype, LITERAL_NAME);
    assert_str_eq(tokens[0]->u.name, "add");
}

static void test_tokenize_one_open_curly(void) {
    char *input = "{";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ltype, OPEN_CURLY);
    assert_int_eq(tokens[0]->u.onechar, '{');
}

static void test_tokenize_one_close_curly(void) {
    char *input = "}";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ltype, CLOSE_CURLY);
    assert_int_eq(tokens[0]->u.onechar, '}');
}

static void test_tokenize_multi_tokens(void) {
    char *input = "{ 123 foo /bar } ";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ltype, OPEN_CURLY);
    assert_int_eq(tokens[1]->ltype, SPACE);
    assert_int_eq(tokens[2]->ltype, NUMBER);
    assert_int_eq(tokens[2]->u.number, 123);
    assert_int_eq(tokens[3]->ltype, SPACE);
    assert_int_eq(tokens[4]->ltype, EXECUTABLE_NAME);
    assert_str_eq(tokens[4]->u.name, "foo");
    assert_int_eq(tokens[5]->ltype, SPACE);
    assert_int_eq(tokens[6]->ltype, LITERAL_NAME);
    assert_str_eq(tokens[6]->u.name, "bar");
    assert_int_eq(tokens[7]->ltype, SPACE);
    assert_int_eq(tokens[8]->ltype, CLOSE_CURLY);
}

void test_tokenize(void) {
    test_tokenize_empty_string();
    test_tokenize_one_number();
    test_tokenize_one_executable_name();
    test_tokenize_one_literal_name();
    test_tokenize_one_open_curly();
    test_tokenize_one_close_curly();
    test_tokenize_multi_tokens();

    printf("%s: OK\n", __func__);
}
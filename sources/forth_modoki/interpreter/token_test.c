#include "clesson.h"

static void test_tokenize_empty_string(void) {
    char *input = "";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_END_OF_FILE);
}

static void test_tokenize_one_number(void) {
    char *input = "123";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_NUMBER);
    assert_int_eq(tokens[0]->u.number, 123);
}

static void test_tokenize_one_executable_name(void) {
    char *input = "add";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_EXECUTABLE_NAME);
    assert_str_eq(tokens[0]->u.name, "add");
}

static void test_tokenize_one_executable_name_with_underscore(void) {
    char *input = "foo_bar";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_EXECUTABLE_NAME);
    assert_str_eq(tokens[0]->u.name, "foo_bar");
}

static void test_tokenize_one_literal_name(void) {
    char *input = "/add";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_LITERAL_NAME);
    assert_str_eq(tokens[0]->u.name, "add");
}

static void test_tokenize_one_open_curly(void) {
    char *input = "{";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_OPEN_CURLY);
    assert_int_eq(tokens[0]->u.onechar, '{');
}

static void test_tokenize_one_close_curly(void) {
    char *input = "}";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_CLOSE_CURLY);
    assert_int_eq(tokens[0]->u.onechar, '}');
}

static void test_tokenize_multi_tokens(void) {
    char *input = "{ 123 foo /bar } ";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_OPEN_CURLY);
    assert_int_eq(tokens[1]->ty, TK_NUMBER);
    assert_int_eq(tokens[1]->u.number, 123);
    assert_int_eq(tokens[2]->ty, TK_EXECUTABLE_NAME);
    assert_str_eq(tokens[2]->u.name, "foo");
    assert_int_eq(tokens[3]->ty, TK_LITERAL_NAME);
    assert_str_eq(tokens[3]->u.name, "bar");
    assert_int_eq(tokens[4]->ty, TK_CLOSE_CURLY);
}

static void test_tokenize_skip_space(void) {
    char *input = " ";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_END_OF_FILE);
}

static void test_tokenize_skip_newline(void) {
    char *input = "\n";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_END_OF_FILE);
}

static void test_tokenize_skip_end_of_newline(void) {
    char *input = "123\n";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_NUMBER);
    assert_int_eq(tokens[0]->u.number, 123);
    assert_int_eq(tokens[1]->ty, TK_END_OF_FILE);
}

static void test_tokenize_skip_comment(void) {
    char *input = "%sThis is comment\n123";
    struct Token **tokens = tokenize(input);

    assert_int_eq(tokens[0]->ty, TK_NUMBER);
    assert_int_eq(tokens[0]->u.number, 123);
    assert_int_eq(tokens[1]->ty, TK_END_OF_FILE);
}

void test_tokenize(void) {
    test_tokenize_empty_string();
    test_tokenize_one_number();
    test_tokenize_one_executable_name();
    test_tokenize_one_executable_name_with_underscore();
    test_tokenize_one_literal_name();
    test_tokenize_one_open_curly();
    test_tokenize_one_close_curly();
    test_tokenize_multi_tokens();
    test_tokenize_skip_space();
    test_tokenize_skip_newline();
    test_tokenize_skip_end_of_newline();
    test_tokenize_skip_comment();

    printf("%s: OK\n", __func__);
}
#include "clesson.h"

#define NAME_SIZE 256

int parse_one(int c, struct Token *out_token) {
    // When parse_one is called for the first time, EOF is specified as an first
    // argument. Therefore we read the one character here.
    if (c == EOF)
        c = cl_getc();

    if (isdigit(c)) {
        // Parse Integer
        int n = c - '0';

        while ((c = cl_getc()) != EOF && isdigit(c))
            n = 10 * n + (c - '0');

        out_token->ltype = NUMBER;
        out_token->u.number = n;
        return c;
    } else if (isspace(c)) {
        // Parse space and skip multiple white space.
        while ((c = cl_getc()) != EOF && isspace(c))
            ;

        out_token->ltype = SPACE;
        return c;
    } else if (isalpha(c) || c == '/') {
        // Parse executable or literal name.
        bool is_literal = (c == '/');
        if (is_literal)
            c = cl_getc();

        char *name = calloc(1, NAME_SIZE);
        int i = 0;
        do {
            name[i++] = c;
        } while ((c = cl_getc()) != EOF && isalpha(c));

        out_token->ltype = is_literal ? LITERAL_NAME : EXECUTABLE_NAME;
        out_token->u.name = name;
        return c;
    } else if (c == '{') {
        out_token->ltype = OPEN_CURLY;
        out_token->u.onechar = c;
        return cl_getc();
    } else if (c == '}') {
        out_token->ltype = CLOSE_CURLY;
        out_token->u.onechar = c;
        return cl_getc();
    }

    out_token->ltype = END_OF_FILE;
    return EOF;
}

void parser_print_all(void) {
    int ch = EOF;
    struct Token token = {UNKNOWN, {0}};

    do {
        ch = parse_one(ch, &token);
        switch (token.ltype) {
        case NUMBER:
            printf("num: %d\n", token.u.number);
            break;
        case SPACE:
            printf("space!\n");
            break;
        case OPEN_CURLY:
            printf("Open curly brace '%c'\n", token.u.onechar);
            break;
        case CLOSE_CURLY:
            printf("Close curly brace '%c'\n", token.u.onechar);
            break;
        case EXECUTABLE_NAME:
            printf("EXECUTABLE_NAME: %s\n", token.u.name);
            break;
        case LITERAL_NAME:
            printf("LITERAL_NAME: %s\n", token.u.name);
            break;

        default:
            printf("Unknown type %d\n", token.ltype);
            break;
        }
    } while (ch != EOF);
}

static void assert_str_eq(const char *s1, const char *s2) {
    assert(strcmp(s1, s2) == 0);
}

static void test_parse_one_number() {
    char *input = "123";
    int expect = 123;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == NUMBER);
    assert(expect == token.u.number);
}

static void test_parse_one_empty_should_return_END_OF_FILE() {
    char *input = "";
    int expect = END_OF_FILE;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}

static void test_parse_one_executable_name() {
    char *input = "add";

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == EXECUTABLE_NAME);
    assert_str_eq(token.u.name, "add");
}

static void test_parse_one_literal_name() {
    char *input = "/add";

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == LITERAL_NAME);
    assert_str_eq(token.u.name, "add");
}

static void test_parse_one_open_curly() {
    char *input = "{";

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == OPEN_CURLY);
    assert(token.u.onechar == '{');
}

static void test_parse_one_close_curly() {
    char *input = "}";

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == CLOSE_CURLY);
    assert(token.u.onechar == '}');
}

static void unit_tests() {
    test_parse_one_empty_should_return_END_OF_FILE();
    test_parse_one_number();
    test_parse_one_executable_name();
    test_parse_one_literal_name();
    test_parse_one_open_curly();
    test_parse_one_close_curly();
}

// int main() {
//     unit_tests();
//
//     cl_getc_set_src("123 45 add /some { 2 3 add } def");
//     parser_print_all();
//     return 0;
// }
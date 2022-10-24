#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "clesson.h"

/*
cc cl_getc.c int_parser_getc.c
*/

enum TokenType {
    NUMBER = 0,
    SPACE,
};

struct Token {
    enum TokenType ty;
    int val;
};

static struct Token *new_token(enum TokenType ty, int val) {
    struct Token *t = calloc(1, sizeof(struct Token));
    t->ty = ty;
    t->val = val;
    return t;
}

static char parse_one(int c, struct Token **out_tok) {
    if (isdigit(c)) {
        int n = c - '0';

        while ((c = cl_getc()) != EOF && isdigit(c))
            n = 10 * n + (c - '0');

        *out_tok = new_token(NUMBER, n);
        return c;
    } else if (isspace(c)) {
        // skip multiple white space
        while ((c = cl_getc()) != EOF && isspace(c))
            ;

        *out_tok = new_token(SPACE, ' ');
        return c;
    }

    // Unsupported input
    assert(false);
}

static void test_parse_one_123(void) {
    cl_getc_set_src("123");
    struct Token *t;
    parse_one(cl_getc(), &t);
    assert(t->ty == NUMBER);
    assert(t->val == 123);
}

static void test_parse_one_123_space_456(void) {
    cl_getc_set_src("123 456");
    struct Token *t1;
    struct Token *t2;
    struct Token *t3;
    int c = parse_one(cl_getc(), &t1);
    c = parse_one(c, &t2);
    c = parse_one(c, &t3);

    // verity result.
    assert(t1->ty == NUMBER);
    assert(t1->val == 123);
    assert(t2->ty == SPACE);
    assert(t2->val == ' ');
    assert(t3->ty == NUMBER);
    assert(t3->val == 456);
}

int main() {
    test_parse_one_123();
    test_parse_one_123_space_456();

    return 0;
}
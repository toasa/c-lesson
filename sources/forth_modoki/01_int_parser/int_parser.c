#include <assert.h>
#include <ctype.h>
#include <stdio.h>

static const char *const input = "123 456  1203";

int single_int_parse(const char **s) {
    int n = 0;

    // Skip white space.
    for (; isspace(**s); (*s)++)
        ;

    for (; isdigit(**s); (*s)++)
        n = n * 10 + **s - '0';

    return n;
}

int main() {
    const char *s = input;

    int answer1 = single_int_parse(&s);
    int answer2 = single_int_parse(&s);
    int answer3 = single_int_parse(&s);

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);
    assert(answer3 == 1203);

    return 0;
}
#include <stdarg.h>

#include "disasm.h"

bool streq(const char *s1, const char *s2) { return strcmp(s1, s2) == 0; }

void assert_int_eq(int x, int y) {
    if (x != y) {
        fprintf(stderr, "%d != %d\n", x, y);
        abort();
    };
}

void assert_str_eq(const char *s1, const char *s2) {
    if (!streq(s1, s2)) {
        fprintf(stderr, "%s != %s\n", s1, s2);
        abort();
    }
}

void expect(bool b, const char *fmt, ...) {
    if (!b) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
        exit(1);
    }
}
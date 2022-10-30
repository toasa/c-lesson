#include "clesson.h"

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

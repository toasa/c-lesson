#include "clesson.h"
#include <string.h>

static const char *input = "123 456";
static int pos = 0;

int cl_getc(void) {
    if (input[pos] == '\0')
        return EOF;
    return input[pos++];
}

void cl_getc_set_src(const char *str) {
    input = str;
    pos = 0;
}

#include "clesson.h"

#define SRCCODE_LEN_MAX 4096

static char *read_file(const char *path) {
    static char input[SRCCODE_LEN_MAX];

    // Read the source code file and interpret it.
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    size_t nread = fread(input, 1, SRCCODE_LEN_MAX, fp);

    if (nread >= SRCCODE_LEN_MAX) {
        fprintf(stderr, "Too large source file: %s", path);
        exit(1);
    }

    // Be null-terminated string.
    input[nread] = '\0';

    return input;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        do_unittests();
        return 0;
    }

    char *input = read_file(argv[1]);
    struct Token **tokens = tokenize(input);
    eval(tokens);

    stack_print_all();
}
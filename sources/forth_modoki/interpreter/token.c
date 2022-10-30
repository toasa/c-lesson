#include "clesson.h"

#define MAX_TOKENS 1024
#define NAME_SIZE 256

struct Token *tokens[MAX_TOKENS];

const char *_input;
static int pos = 0;

static char _getc(void) { return _input[pos++]; }
static void _back(void) { pos--; }
static void _init(const char *input) {
    _input = input;
    pos = 0;
}

struct Token **tokenize(const char *input) {
    _init(input);

    int len = 0;
    for (char c = _getc(); c != '\0'; c = _getc()) {
        if (isdigit(c)) {
            // Parse Integer
            int n = 0;
            do {
                n = 10 * n + (c - '0');
                c = _getc();
            } while (isdigit(c));
            _back();

            tokens[len] = new_token(NUMBER);
            tokens[len]->u.number = n;
        } else if (isspace(c)) {
            // Parse space and skip multiple white space.
            while ((c = _getc()) != '\0' && isspace(c))
                ;
            _back();

            tokens[len] = new_token(SPACE);
        } else if (isalpha(c) || c == '/') {
            // Parse executable or literal name.
            bool is_literal = (c == '/');
            if (is_literal)
                c = _getc();

            char *name = calloc(1, NAME_SIZE);
            for (int i = 0; isalpha(c); i++, c = _getc())
                name[i] = c;
            _back();

            tokens[len] =
                new_token(is_literal ? LITERAL_NAME : EXECUTABLE_NAME);
            tokens[len]->u.name = name;
        } else if (c == '{') {
            tokens[len] = new_token(OPEN_CURLY);
            tokens[len]->u.onechar = c;
        } else if (c == '}') {
            tokens[len] = new_token(CLOSE_CURLY);
            tokens[len]->u.onechar = c;
        }

        len++;
    }

    tokens[len] = new_token(END_OF_FILE);
    return tokens;
}

void token_print(struct Token *t) {
    switch (t->ltype) {
    case NUMBER:
        printf("num: %d\n", t->u.number);
        break;
    case SPACE:
        printf("space!\n");
        break;
    case OPEN_CURLY:
        printf("Open curly brace '%c'\n", t->u.onechar);
        break;
    case CLOSE_CURLY:
        printf("Close curly brace '%c'\n", t->u.onechar);
        break;
    case EXECUTABLE_NAME:
        printf("EXECUTABLE_NAME: %s\n", t->u.name);
        break;
    case LITERAL_NAME:
        printf("LITERAL_NAME: %s\n", t->u.name);
        break;
    default:
        printf("Unknown type %d\n", t->ltype);
        break;
    }
}

struct Token *new_token(enum LexicalType ty) {
    struct Token *t = calloc(1, sizeof(struct Token));
    t->ltype = ty;
    return t;
}

struct Token *new_num_token(int val) {
    struct Token *t = new_token(NUMBER);
    t->u.number = val;
    return t;
}

struct Token *new_literal_token(char *name) {
    struct Token *t = new_token(LITERAL_NAME);
    t->u.name = name;
    return t;
}
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

static bool isnewline(char c) { return c == '\n'; }

struct Token **tokenize(const char *input) {
    _init(input);

    int len = 0;
    for (char c = _getc(); c != '\0'; c = _getc()) {
        // Skip white space
        if (isspace(c) || isnewline(c)) {
            while ((c = _getc()) != '\0' && (isspace(c) || isnewline(c)))
                ;
            _back();
            continue;
        }

        // Skip comment
        if (c == '%') {
            while ((c = _getc()) != '\0' && c != '\n')
                ;
            _back();
            continue;
        }

        if (isdigit(c)) {
            // Parse Integer
            int n = 0;
            do {
                n = 10 * n + (c - '0');
                c = _getc();
            } while (isdigit(c));
            _back();

            tokens[len] = new_token(TK_NUMBER);
            tokens[len]->u.number = n;
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
                new_token(is_literal ? TK_LITERAL_NAME : TK_EXECUTABLE_NAME);
            tokens[len]->u.name = name;
        } else if (c == '{') {
            tokens[len] = new_token(TK_OPEN_CURLY);
            tokens[len]->u.onechar = c;
        } else if (c == '}') {
            tokens[len] = new_token(TK_CLOSE_CURLY);
            tokens[len]->u.onechar = c;
        }

        len++;
    }

    tokens[len] = new_token(TK_END_OF_FILE);
    return tokens;
}

void token_print(struct Token *t) {
    switch (t->ty) {
    case TK_NUMBER:
        printf("num: %d\n", t->u.number);
        break;
    case TK_OPEN_CURLY:
        printf("Open curly brace '%c'\n", t->u.onechar);
        break;
    case TK_CLOSE_CURLY:
        printf("Close curly brace '%c'\n", t->u.onechar);
        break;
    case TK_EXECUTABLE_NAME:
        printf("EXECUTABLE_NAME: %s\n", t->u.name);
        break;
    case TK_LITERAL_NAME:
        printf("LITERAL_NAME: %s\n", t->u.name);
        break;
    default:
        printf("Unknown type %d\n", t->ty);
        break;
    }
}

struct Token *new_token(enum TokenType ty) {
    struct Token *t = calloc(1, sizeof(struct Token));
    t->ty = ty;
    return t;
}

struct Token *new_num_token(int val) {
    struct Token *t = new_token(TK_NUMBER);
    t->u.number = val;
    return t;
}

struct Token *new_literal_token(char *name) {
    struct Token *t = new_token(TK_LITERAL_NAME);
    t->u.name = name;
    return t;
}
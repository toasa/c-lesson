#include "clesson.h"

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
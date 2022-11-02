#include "clesson.h"

struct Element *new_num_element(int num) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->ty = ELEM_NUMBER;
    e->u.number = num;
    return e;
}

struct Element *new_lit_name_element(char *name) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->ty = ELEM_LITERAL_NAME;
    e->u.name = name;
    return e;
}

struct Element *new_exec_name_element(char *name) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->ty = ELEM_EXECUTABLE_NAME;
    e->u.name = name;
    return e;
}

struct Element *new_cfunc_element(void (*cfunc)()) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->ty = ELEM_C_FUNC;
    e->u.cfunc = cfunc;
    return e;
}

struct Element *new_exec_array_element(struct ElementArray *ea) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->ty = ELEM_EXECUTABLE_ARRAY;
    e->u.byte_code = ea;
    return e;
}

struct Element *copy_element(struct Element *e) {
    struct Element *new = calloc(1, sizeof(struct Element));
    *new = *e;
    return new;
}

static void _element_print(struct Element *e, bool newline) {
    switch (e->ty) {
    case ELEM_NUMBER:
        printf("Number: %d", e->u.number);
        break;
    case ELEM_LITERAL_NAME:
        printf("Lit name: %s", e->u.name);
        break;
    case ELEM_EXECUTABLE_NAME:
        printf("Exec name: %s", e->u.name);
        break;
    case ELEM_C_FUNC:
        printf("C Func");
        break;
    case ELEM_EXECUTABLE_ARRAY: {
        printf("Exec arr: {");
        struct ElementArray *ea = e->u.byte_code;
        for (int i = 0; i < ea->len; i++) {
            _element_print(ea->elem[i], false);
            if (i + 1 != ea->len)
                printf(" ");
        }
        printf("}");
        break;
    }
    default:
        printf("Unknown type %d", e->ty);
        break;
    }

    if (newline)
        printf("\n");
}

void element_print(struct Element *e) { _element_print(e, true); }
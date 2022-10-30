#include "clesson.h"

struct Element *new_num_element(int num) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->etype = ELEM_NUMBER;
    e->u.number = num;
    return e;
}

struct Element *new_lit_name_element(char *name) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->etype = ELEM_LITERAL_NAME;
    e->u.name = name;
    return e;
}

struct Element *new_exec_name_element(char *name) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->etype = ELEM_EXECUTABLE_NAME;
    e->u.name = name;
    return e;
}

struct Element *new_cfunc_element(void (*cfunc)()) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->etype = ELEM_C_FUNC;
    e->u.cfunc = cfunc;
    return e;
}

struct Element *new_exec_array_element(struct ElementArray *ea) {
    struct Element *e = calloc(1, sizeof(struct Element));
    e->etype = ELEM_EXECUTABLE_ARRAY;
    e->u.byte_code = ea;
    return e;
}

void element_print(struct Element *e) {
    switch (e->etype) {
    case ELEM_NUMBER:
        printf("num: %d\n", e->u.number);
        break;
    case ELEM_LITERAL_NAME:
        printf("LITERAL NAME: %s\n", e->u.name);
        break;
    default:
        printf("Unknown type %d\n", e->etype);
        break;
    }
}
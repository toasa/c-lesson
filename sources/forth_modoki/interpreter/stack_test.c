#include "clesson.h"

static void test_pop_only(void) {
    struct Element *e = stack_pop();
    assert(e == NULL);
}

static void test_push_and_pop(void) {
    struct Element *e_in = new_num_element(10);

    stack_push(e_in);
    struct Element *e_out = stack_pop();

    assert(e_out->u.number == 10);
}

static void test_double_push_and_pop(void) {
    struct Element *e1_in = new_num_element(10);
    struct Element *e2_in = new_num_element(20);

    stack_push(e1_in);
    stack_push(e2_in);

    struct Element *e2_out = stack_pop();
    struct Element *e1_out = stack_pop();

    assert(e2_out->u.number == 20);
    assert(e1_out->u.number == 10);
}

void test_stack(void) {
    test_pop_only();
    test_push_and_pop();
    test_double_push_and_pop();

    printf("%s: OK\n", __func__);
}
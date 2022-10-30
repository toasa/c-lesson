#include "clesson.h"

static void test_dict_get_no_exist(void) {
    struct Element *e = dict_get("nothing");

    assert(e == NULL);
}

static void test_dict_put_get(void) {
    struct Element *e_in = new_num_element(10);
    dict_put("foo", e_in);

    struct Element *e_out = dict_get("foo");

    assert(e_out->u.number == 10);
}

static void test_dict_multi_put_get(void) {
    struct Element *e1_in = new_num_element(10);
    struct Element *e2_in = new_num_element(20);
    struct Element *e3_in = new_num_element(30);
    dict_put("foo", e1_in);
    dict_put("bar", e2_in);
    dict_put("baz", e3_in);

    struct Element *e1_out = dict_get("foo");
    struct Element *e2_out = dict_get("bar");
    struct Element *e3_out = dict_get("baz");

    assert(e1_out->u.number == 10);
    assert(e2_out->u.number == 20);
    assert(e3_out->u.number == 30);
}

static void test_dict_multi_put_get_conflict(void) {
    struct Element *e1_in = new_num_element(10);
    struct Element *e2_in = new_num_element(20);
    struct Element *e3_in = new_num_element(30);
    dict_put("bar", e1_in);
    dict_put("arb", e2_in);
    dict_put("rba", e3_in);

    struct Element *e1_out = dict_get("bar");
    struct Element *e2_out = dict_get("arb");
    struct Element *e3_out = dict_get("rba");

    assert(e1_out->u.number == 10);
    assert(e2_out->u.number == 20);
    assert(e3_out->u.number == 30);
}

static void test_dict_val_update(void) {
    struct Element *e1 = new_num_element(10);
    struct Element *e2 = new_num_element(20);

    dict_put("foo", e1);
    dict_put("foo", e2);

    struct Element *e_out = dict_get("foo");

    assert(e_out->u.number == 20);
}

void test_dict(void) {
    test_dict_get_no_exist();
    test_dict_put_get();
    test_dict_multi_put_get();
    test_dict_multi_put_get_conflict();
    test_dict_val_update();

    printf("%s: OK\n", __func__);
}
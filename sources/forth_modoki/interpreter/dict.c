#include "clesson.h"

struct Node {
    const char *key;
    struct Element *val;
    struct Node *next;
};

struct Node *new_node(const char *key, struct Element *val) {
    struct Node *n = calloc(1, sizeof(struct Node));
    n->key = key;
    n->val = val;
    return n;
}

#define TABLE_SIZE 1024
static struct Node *node_array[TABLE_SIZE];

int hash(const char *s) {
    unsigned val = 0;
    while (*s)
        val += *s++;
    return (int)(val % TABLE_SIZE);
}

void dict_put(const char *key, struct Element *val) {
    int idx = hash(key);

    struct Node *head = node_array[idx];
    if (head == NULL) {
        struct Node *n = new_node(key, val);
        n->next = NULL;
        node_array[idx] = n;
        return;
    }

    struct Node *cur = head;
    for (;; cur = cur->next) {
        if (streq(cur->key, key)) {
            // update value
            cur->val = val;
            return;
        }

        if (cur->next == NULL)
            break;
    }

    // Insert at the tail
    struct Node *n = new_node(key, val);
    cur->next = n;
}

int dict_get(const char *key, struct Element *out_elem) {
    int idx = hash(key);

    struct Node *head = node_array[idx];
    if (head == NULL)
        return 0;

    struct Node *cur = head;
    for (; cur != NULL; cur = cur->next) {
        if (streq(cur->key, key)) {
            *out_elem = *cur->val;
            return 1;
        }
    }

    return 0;
}

void dict_print_all(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct Node *head = node_array[i];
        if (head == NULL)
            continue;

        printf("hash val: %d\n", hash(head->key));
        for (struct Node *cur = head; cur != NULL; cur = cur->next) {
            printf("  key: %s", cur->key);
            printf("  val: ");
            element_print(cur->val);
            printf("  ->\n");
        }
    }
}

static void test_dict_get_no_exist(void) {
    struct Element e;
    int res = dict_get("nothing", &e);

    assert(res == 0);
}

static void test_dict_put_get(void) {
    struct Element *e_in = new_num_element(10);
    dict_put("foo", e_in);

    struct Element e_out;
    int res = dict_get("foo", &e_out);

    assert(res == 1);
    assert(e_out.u.number == 10);
}

static void test_dict_multi_put_get(void) {
    struct Element *e1_in = new_num_element(10);
    struct Element *e2_in = new_num_element(20);
    struct Element *e3_in = new_num_element(30);
    dict_put("foo", e1_in);
    dict_put("bar", e2_in);
    dict_put("baz", e3_in);

    struct Element e1_out;
    struct Element e2_out;
    struct Element e3_out;
    assert(dict_get("foo", &e1_out) == 1);
    assert(dict_get("bar", &e2_out) == 1);
    assert(dict_get("baz", &e3_out) == 1);

    assert(e1_out.u.number == 10);
    assert(e2_out.u.number == 20);
    assert(e3_out.u.number == 30);
}

static void test_dict_multi_put_get_conflict(void) {
    struct Element *e1_in = new_num_element(10);
    struct Element *e2_in = new_num_element(20);
    struct Element *e3_in = new_num_element(30);
    dict_put("bar", e1_in);
    dict_put("arb", e2_in);
    dict_put("rba", e3_in);

    struct Element e1_out;
    struct Element e2_out;
    struct Element e3_out;
    assert(dict_get("bar", &e1_out) == 1);
    assert(dict_get("arb", &e2_out) == 1);
    assert(dict_get("rba", &e3_out) == 1);

    assert(e1_out.u.number == 10);
    assert(e2_out.u.number == 20);
    assert(e3_out.u.number == 30);
}

static void test_dict_val_update(void) {
    struct Element *e1 = new_num_element(10);
    struct Element *e2 = new_num_element(20);

    dict_put("foo", e1);
    dict_put("foo", e2);

    struct Element e_out;
    int res = dict_get("foo", &e_out);

    assert(res == 1);
    assert(e_out.u.number == 20);
}

int main() {
    test_dict_get_no_exist();
    test_dict_put_get();
    test_dict_multi_put_get();
    test_dict_multi_put_get_conflict();
    test_dict_val_update();

    printf("OK\n");

    // dict_print_all();
}
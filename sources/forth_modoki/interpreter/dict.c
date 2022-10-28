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

struct Element *dict_get(const char *key) {
    int idx = hash(key);

    struct Node *head = node_array[idx];
    if (head == NULL)
        return NULL;

    struct Node *cur = head;
    for (; cur != NULL; cur = cur->next) {
        if (streq(cur->key, key)) {
            return cur->val;
        }
    }

    return NULL;
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
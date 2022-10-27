#include "clesson.h"

struct Node {
    const char *key;
    struct Token *val;
    struct Node *next;
};

struct Node *new_node(const char *key, struct Token *val) {
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

void dict_put(const char *key, struct Token *val) {
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

int dict_get(const char *key, struct Token *out_elem) {
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
            token_print(cur->val);
            printf("  ->\n");
        }
    }
}

static void test_dict_get_no_exist(void) {
    struct Token t;
    int res = dict_get("nothing", &t);

    assert(res == 0);
}

static void test_dict_put_get(void) {
    struct Token *t_in = new_num_token(10);
    dict_put("foo", t_in);

    struct Token t_out;
    int res = dict_get("foo", &t_out);

    assert(res == 1);
    assert(t_out.u.number == 10);
}

static void test_dict_multi_put_get(void) {
    struct Token *t1_in = new_num_token(10);
    struct Token *t2_in = new_num_token(20);
    struct Token *t3_in = new_num_token(30);
    dict_put("foo", t1_in);
    dict_put("bar", t2_in);
    dict_put("baz", t3_in);

    struct Token t1_out;
    struct Token t2_out;
    struct Token t3_out;
    assert(dict_get("foo", &t1_out) == 1);
    assert(dict_get("bar", &t2_out) == 1);
    assert(dict_get("baz", &t3_out) == 1);

    assert(t1_out.u.number == 10);
    assert(t2_out.u.number == 20);
    assert(t3_out.u.number == 30);
}

static void test_dict_multi_put_get_conflict(void) {
    struct Token *t1_in = new_num_token(10);
    struct Token *t2_in = new_num_token(20);
    struct Token *t3_in = new_num_token(30);
    dict_put("bar", t1_in);
    dict_put("arb", t2_in);
    dict_put("rba", t3_in);

    struct Token t1_out;
    struct Token t2_out;
    struct Token t3_out;
    assert(dict_get("bar", &t1_out) == 1);
    assert(dict_get("arb", &t2_out) == 1);
    assert(dict_get("rba", &t3_out) == 1);

    assert(t1_out.u.number == 10);
    assert(t2_out.u.number == 20);
    assert(t3_out.u.number == 30);
}

static void test_dict_val_update(void) {
    struct Token *t1 = new_num_token(10);
    struct Token *t2 = new_num_token(20);

    dict_put("foo", t1);
    dict_put("foo", t2);

    struct Token t_out;
    int res = dict_get("foo", &t_out);

    assert(res == 1);
    assert(t_out.u.number == 20);
}

int main() {
    test_dict_get_no_exist();
    test_dict_put_get();
    test_dict_multi_put_get();
    test_dict_multi_put_get_conflict();
    test_dict_val_update();

    // dict_print_all();
}
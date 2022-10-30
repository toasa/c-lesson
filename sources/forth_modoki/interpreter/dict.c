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
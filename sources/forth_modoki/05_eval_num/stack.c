#include <assert.h>
#include <stdio.h>

#include "stack.h"

#define STACK_SIZE 1024

static struct Token *stack[STACK_SIZE];
static unsigned head = 0;

void stack_push(struct Token *t) {
    stack[head] = t;
    head++;
}

struct Token *stack_pop(void) {
    if (head == 0)
        return NULL;
    head--;
    return stack[head];
};

bool stack_is_empty(void) { return head == 0; }

void stack_print_all(void) {
    for (unsigned i = 0; i < head; i++)
        token_print(stack[i]);
}

static void test_pop_only(void) {
    struct Token *t = stack_pop();
    assert(t == NULL);
}

static void test_push_and_pop(void) {
    struct Token *t_in = new_num_token(10);

    stack_push(t_in);
    struct Token *t_out = stack_pop();

    assert(t_out->u.number == 10);
}

static void test_double_push_and_pop(void) {
    struct Token *t1_in = new_num_token(10);
    struct Token *t2_in = new_num_token(20);

    stack_push(t1_in);
    stack_push(t2_in);

    struct Token *t2_out = stack_pop();
    struct Token *t1_out = stack_pop();

    assert(t2_out->u.number == 20);
    assert(t1_out->u.number == 10);
}

static void do_unittests(void) {
    test_pop_only();
    test_push_and_pop();
    test_double_push_and_pop();

    printf("OK\n");
}

// int main() {
//     do_unittests();
//     return 0;
// }
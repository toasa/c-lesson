#include "clesson.h"

#define STACK_SIZE 1024

static struct Element *stack[STACK_SIZE];
static int head = 0;

void stack_push(struct Element *e) {
    expect(head < STACK_SIZE, "Stack overflow");
    stack[head++] = e;
}

struct Element *stack_pop(void) {
    if (head == 0)
        return NULL;
    return stack[--head];
};

void stack_init(void) { head = 0; }

bool stack_is_empty(void) { return head == 0; }

void stack_print_all(void) {
    for (int i = head - 1; i >= 0; i--)
        element_print(stack[i]);
}

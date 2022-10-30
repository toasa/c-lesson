#include "clesson.h"

#define STACK_SIZE 1024

static struct Element *stack[STACK_SIZE];
static unsigned head = 0;

void stack_push(struct Element *e) {
    stack[head] = e;
    head++;
}

struct Element *stack_pop(void) {
    if (head == 0)
        return NULL;
    head--;
    return stack[head];
};

bool stack_is_empty(void) { return head == 0; }

void stack_print_all(void) {
    for (unsigned i = 0; i < head; i++)
        element_print(stack[i]);
}

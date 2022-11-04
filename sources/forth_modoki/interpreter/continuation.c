#include "clesson.h"

#define CONTINUATION_STACK_SIZE 1024

static struct Continuation *co_stack[CONTINUATION_STACK_SIZE];
static int pos = 0;

struct Continuation *new_co(struct ElementArray *ea, int pc) {
    struct Continuation *c = calloc(1, sizeof(struct Continuation));
    c->exec_array = ea;
    c->pc = pc;
    return c;
}

void co_push(struct Continuation *c) {
    expect(pos < CONTINUATION_STACK_SIZE, "Continuation stack overflow");
    co_stack[pos++] = c;
}

struct Continuation *co_pop(void) {
    expect(pos >= 0, "Continuation stack is empty");
    return co_stack[--pos];
}

bool co_stack_empty(void) { return pos == 0; }
#include "clesson.h"

#define CONTINUATION_STACK_SIZE 1024

static struct ContStackElement *co_stack[CONTINUATION_STACK_SIZE];
static int pos = 0;

struct ContStackElement *new_co(struct ElementArray *ea, int pc) {
    struct Continuation *co = calloc(1, sizeof(struct Continuation));
    co->exec_array = ea;
    co->pc = pc;

    struct ContStackElement *c = calloc(1, sizeof(struct ContStackElement));
    c->ty = CS_ELEM_CONTINUATION;
    c->u.co = co;
    return c;
}

struct ContStackElement *new_lvar(struct Element *lvar) {
    struct ContStackElement *c = calloc(1, sizeof(struct ContStackElement));
    c->ty = CS_ELEM_LOCAL_VAR;
    c->u.lvar = lvar;
    return c;
}

void co_push(struct ContStackElement *c) {
    expect(pos < CONTINUATION_STACK_SIZE, "Continuation stack overflow");
    co_stack[pos++] = c;
}

struct ContStackElement *co_pop(void) {
    if (pos <= 0)
        return NULL;
    return co_stack[--pos];
}

struct Element *co_load_lvar(int n) {
    expect(co_stack[pos - n - 1]->ty == CS_ELEM_LOCAL_VAR,
           "Load Op must be on lvar");
    return co_stack[pos - n - 1]->u.lvar;
}

bool co_stack_empty(void) { return pos == 0; }
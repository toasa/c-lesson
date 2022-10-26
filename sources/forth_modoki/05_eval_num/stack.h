#ifndef STACK_H_
#define STACK_H_

#include <stdbool.h>

#include "token.h"

void stack_push(struct Token *t);

struct Token *stack_pop(void);

bool stack_is_empty(void);

void stack_print_all(void);

#endif
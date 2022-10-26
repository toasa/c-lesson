#ifndef PARSER_H_
#define PARSER_H_

#include "token.h"

int parse_one(int c, struct Token *out_token);

void parser_print_all(void);

#endif
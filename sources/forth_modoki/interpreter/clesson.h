#ifndef _CLESSON_H_
#define _CLESSON_H_

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// cl_getc.c
//
int cl_getc(void);
void cl_getc_set_src(char *str);

//
// token.c
//

enum LexicalType {
    NUMBER,
    SPACE,
    EXECUTABLE_NAME,
    LITERAL_NAME,
    OPEN_CURLY,
    CLOSE_CURLY,
    END_OF_FILE,
    UNKNOWN,
};

struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};

void token_print(struct Token *t);

struct Token *new_token(enum LexicalType ty);
struct Token *new_num_token(int val);
struct Token *new_literal_token(char *name);
struct Token *copy(struct Token *t);

//
// parser.c
//

int parse_one(int c, struct Token *out_token);
void parser_print_all(void);
void test_parser(void);

//
// eval.c
//
void test_eval(void);

//
// elem.c
//

enum ElementType {
    ELEM_NUMBER,
    ELEM_LITERAL_NAME,
};

struct Element {
    enum ElementType etype;
    union {
        int number;
        char *name;
    } u;
};

struct Element *new_num_element(int num);
struct Element *new_lit_name_element(char *name);
struct Element *copy_element(struct Element *e);
void element_print(struct Element *e);

//
// dict.c
//

void dict_put(const char *key, struct Element *elem);
int dict_get(const char *key, struct Element *out_elem);
void dict_print_all(void);
void test_dict(void);

//
// stack.c
//

void stack_push(struct Element *e);
struct Element *stack_pop(void);
bool stack_is_empty(void);
void stack_print_all(void);
void test_stack(void);

//
// util.c
//

bool streq(const char *s1, const char *s2);

#endif
#ifndef _CLESSON_H_
#define _CLESSON_H_

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// token.c
//

enum TokenType {
    TK_NUMBER,
    TK_EXECUTABLE_NAME,
    TK_LITERAL_NAME,
    TK_OPEN_CURLY,
    TK_CLOSE_CURLY,
    TK_END_OF_FILE,
    TK_UNKNOWN,
};

struct Token {
    enum TokenType ty;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};

struct Token **tokenize(const char *input);
void token_print(struct Token *t);
void test_tokenize(void);

struct Token *new_token(enum TokenType ty);
struct Token *new_num_token(int val);
struct Token *new_literal_token(char *name);

//
// eval.c
//

void eval(struct Token *tokens[]);
void test_eval(void);

//
// elem.c
//

enum ElementType {
    ELEM_NUMBER,
    ELEM_LITERAL_NAME,
    ELEM_EXECUTABLE_NAME,
    ELEM_C_FUNC,
    ELEM_EXECUTABLE_ARRAY,
    ELEM_CONTROL,
};

struct ElementArray {
    int len;
    struct Element **elem;
};

struct Element {
    enum ElementType ty;
    union {
        int number;
        char *name;
        void (*cfunc)();
        struct ElementArray *byte_code;
    } u;
};

struct Emitter {
    struct Element **elems;
    int len;
    int cap;
};

struct Element *new_num_element(int num);
struct Element *new_lit_name_element(char *name);
struct Element *new_exec_name_element(char *name);
struct Element *new_cfunc_element(void (*cfunc)());
struct Element *new_exec_array_element(struct ElementArray *ea);
struct Element *new_control_element(char *name);
struct Element *copy_element(struct Element *e);
void element_print(struct Element *e);

struct Emitter *new_emitter(void);
void emit_elem(struct Emitter *em, struct Element *e);
struct ElementArray *emit_get(struct Emitter *em);

//
// dict.c
//

void dict_put(const char *key, struct Element *elem);
struct Element *dict_get(const char *key);
void dict_print_all(void);
void test_dict(void);

//
// stack.c
//

void stack_push(struct Element *e);
struct Element *stack_pop(void);
void stack_init(void);
bool stack_is_empty(void);
void stack_print_all(void);
void test_stack(void);

//
// continuation.c
//

struct Continuation {
    struct ElementArray *exec_array;
    int pc;
};

struct Continuation *new_co(struct ElementArray *ea, int pc);
void co_push(struct Continuation *c);
struct Continuation *co_pop(void);
bool co_stack_empty(void);

//
// util.c
//

bool streq(const char *s1, const char *s2);
void assert_int_eq(int x, int y);
void assert_str_eq(const char *s1, const char *s2);
void expect(bool b, const char *fmt, ...);

//
// test.c
//

void do_unittests(void);

#endif
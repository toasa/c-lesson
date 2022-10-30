#include "clesson.h"

static void add_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number + rhs->u.number));
}

static void sub_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number - rhs->u.number));
}

static void mul_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number * rhs->u.number));
}

static void div_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number / rhs->u.number));
}

static void def(void) {
    struct Element *val = stack_pop();
    struct Element *key = stack_pop();
    dict_put(key->u.name, val);
}

static void register_primitives(void) {
    dict_put("add", new_cfunc_element(add_op));
    dict_put("sub", new_cfunc_element(sub_op));
    dict_put("mul", new_cfunc_element(mul_op));
    dict_put("div", new_cfunc_element(div_op));
    dict_put("def", new_cfunc_element(def));
}

static struct Token **_tokens;
static int pos = 0;

static void _init(struct Token **tokens) {
    _tokens = tokens;
    pos = 0;
    register_primitives();
}

static struct Element *compile_exec_array(void) {
    // Default capacity, expand when needed.
    int cap = 10;
    struct Element **elems = calloc(1, sizeof(struct Element) * cap);

    int len = 0;
    for (; _tokens[pos]->ltype != END_OF_FILE; pos++) {
        struct Token *t = _tokens[pos];

        switch (t->ltype) {
        case NUMBER:
            elems[len++] = new_num_element(t->u.number);
            break;
        case EXECUTABLE_NAME:
            elems[len++] = new_exec_name_element(t->u.name);
            break;
        case LITERAL_NAME:
            elems[len++] = new_lit_name_element(t->u.name);
            break;
        case OPEN_CURLY:
            pos++;
            elems[len++] = compile_exec_array();
            break;
        case CLOSE_CURLY:
            goto succuess;
        default:
            break;
        }

        if (len >= cap) {
            cap *= 2;
            elems = realloc(elems, sizeof(struct Element) * cap);
        }
    }

    // Unreachable point
    abort();

succuess:

    // Shrink the `elems` size to `len`.
    elems = realloc(elems, sizeof(struct Element) * len);

    struct ElementArray *ea = calloc(1, sizeof(struct ElementArray));
    ea->len = len;
    ea->elem = elems;

    return new_exec_array_element(ea);
}

static void eval_elem(struct Element *e);
static void eval_exec_name(const char *name);
static void eval_exec_array(struct ElementArray *elems);

static void eval_exec_name(const char *name) {
    struct Element *e = dict_get(name);
    if (e == NULL) {
        fprintf(stderr, "Unbind variable: %s\n", name);
        abort();
    }

    eval_elem(e);
}

static void eval_elem(struct Element *e) {
    switch (e->etype) {
    case ELEM_NUMBER:
    case ELEM_LITERAL_NAME:
        stack_push(e);
        break;
    case ELEM_EXECUTABLE_NAME:
        eval_exec_name(e->u.name);
        break;
    case ELEM_C_FUNC:
        e->u.cfunc();
        break;
    case ELEM_EXECUTABLE_ARRAY:
        eval_exec_array(e->u.byte_code);
        break;
    default:
        abort();
    }
}

static void eval_exec_array(struct ElementArray *elems) {
    for (int i = 0; i < elems->len; i++)
        eval_elem(elems->elem[i]);
}

void eval(struct Token *tokens[]) {
    _init(tokens);

    for (; _tokens[pos]->ltype != END_OF_FILE; pos++) {
        struct Token *t = _tokens[pos];
        switch (t->ltype) {
        case NUMBER:
            stack_push(new_num_element(t->u.number));
            break;
        case EXECUTABLE_NAME:
            eval_exec_name(t->u.name);
            break;
        case LITERAL_NAME:
            stack_push(new_lit_name_element(t->u.name));
            break;
        case OPEN_CURLY:
            pos++;
            stack_push(compile_exec_array());
            break;
        default:
            break;
        }
    }

    // if (!stack_is_empty())
    //     stack_print_all();
}

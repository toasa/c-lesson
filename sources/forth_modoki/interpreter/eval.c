#include "clesson.h"

static void eval_elem(struct Element *e);
static void eval_exec_name(const char *name);
static void eval_exec_array(struct ElementArray *elems);

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

static void mod_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number % rhs->u.number));
}

static void eq_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number == rhs->u.number));
}

static void neq_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number != rhs->u.number));
}

static void gt_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number > rhs->u.number));
}

static void ge_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number >= rhs->u.number));
}

static void lt_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number < rhs->u.number));
}

static void le_op(void) {
    struct Element *rhs = stack_pop();
    struct Element *lhs = stack_pop();
    stack_push(new_num_element(lhs->u.number <= rhs->u.number));
}

static void def_op(void) {
    struct Element *val = stack_pop();
    struct Element *key = stack_pop();
    dict_put(key->u.name, val);
}

static void pop_op(void) { stack_pop(); }

static void exch_op(void) {
    struct Element *e1 = stack_pop();
    struct Element *e2 = stack_pop();
    stack_push(e1);
    stack_push(e2);
}

static void dup_op(void) {
    struct Element *e1 = stack_pop();
    struct Element *e2 = copy_element(e1);
    stack_push(e1);
    stack_push(e2);
}

static void index_op(void) {
    int n = stack_pop()->u.number;
    struct Element **elems = malloc(sizeof(struct Element *) * n);
    for (int i = 0; i < n; i++)
        elems[i] = stack_pop();

    struct Element *e = stack_pop();
    struct Element *new = copy_element(e);
    stack_push(e);

    for (int i = 0; i < n; i++)
        stack_push(elems[n - (i + 1)]);

    stack_push(new);

    free(elems);
}

static void roll_op(void) {
    int rot_n = stack_pop()->u.number;
    int n = stack_pop()->u.number;
    rot_n %= n;

    struct Element **elems = malloc(sizeof(struct Element *) * n);

    for (int i = 0; i < n; i++)
        elems[n - (i + 1)] = stack_pop();

    for (int i = 0; i < rot_n; i++) {
        struct Element *last = elems[n - 1];
        for (int j = n - 2; j >= 0; j--)
            elems[j + 1] = elems[j];
        elems[0] = last;
    }

    for (int i = 0; i < n; i++)
        stack_push(elems[i]);

    free(elems);
}

static void exec_op(void) {
    struct Element *e = stack_pop();
    expect(e->ty == ELEM_EXECUTABLE_ARRAY,
           "exec's operand must be  executable array.");
    eval_exec_array(e->u.byte_code);
}

static void if_op(void) {
    struct Element *proc = stack_pop();
    struct Element *cond = stack_pop();

    expect(proc->ty == ELEM_EXECUTABLE_ARRAY,
           "if op's body must be executable array.");

    if (cond->u.number)
        eval_exec_array(proc->u.byte_code);
}

static void ifelse_op(void) {
    struct Element *proc_else = stack_pop();
    struct Element *proc_then = stack_pop();
    struct Element *cond = stack_pop();

    expect(proc_then->ty == ELEM_EXECUTABLE_ARRAY,
           "ifelse op's then body must be executable array.");
    expect(proc_else->ty == ELEM_EXECUTABLE_ARRAY,
           "ifelse op's else body must be executable array.");

    if (cond->u.number)
        eval_exec_array(proc_then->u.byte_code);
    else
        eval_exec_array(proc_else->u.byte_code);
}

static void repeat_op(void) {
    struct Element *proc = stack_pop();
    struct Element *n = stack_pop();

    expect(proc->ty == ELEM_EXECUTABLE_ARRAY,
           "repeat op's body must be executable array.");

    for (int i = 0; i < n->u.number; i++)
        eval_exec_array(proc->u.byte_code);
}

static void while_op(void) {
    struct Element *body_proc = stack_pop();
    struct Element *cond_proc = stack_pop();

    expect(body_proc->ty == ELEM_EXECUTABLE_ARRAY,
           "while op's body must be executable array.");
    expect(cond_proc->ty == ELEM_EXECUTABLE_ARRAY,
           "while op's cond must be executable array.");

    while (1) {
        eval_exec_array(cond_proc->u.byte_code);
        if (stack_pop()->u.number)
            eval_exec_array(body_proc->u.byte_code);
        else
            break;
    }
}

static void register_primitives(void) {
    dict_put("add", new_cfunc_element(add_op));
    dict_put("sub", new_cfunc_element(sub_op));
    dict_put("mul", new_cfunc_element(mul_op));
    dict_put("div", new_cfunc_element(div_op));
    dict_put("mod", new_cfunc_element(mod_op));

    dict_put("eq", new_cfunc_element(eq_op));
    dict_put("neq", new_cfunc_element(neq_op));
    dict_put("gt", new_cfunc_element(gt_op));
    dict_put("ge", new_cfunc_element(ge_op));
    dict_put("lt", new_cfunc_element(lt_op));
    dict_put("le", new_cfunc_element(le_op));

    dict_put("def", new_cfunc_element(def_op));

    dict_put("pop", new_cfunc_element(pop_op));
    dict_put("exch", new_cfunc_element(exch_op));
    dict_put("dup", new_cfunc_element(dup_op));
    dict_put("index", new_cfunc_element(index_op));
    dict_put("roll", new_cfunc_element(roll_op));

    dict_put("exec", new_cfunc_element(exec_op));
    dict_put("if", new_cfunc_element(if_op));
    dict_put("ifelse", new_cfunc_element(ifelse_op));
    dict_put("repeat", new_cfunc_element(repeat_op));
    dict_put("while", new_cfunc_element(while_op));
}

// Index of the token sequence currently being read.
static int tok_pos = 0;

static void _init(void) {
    tok_pos = 0;
    stack_init();
    register_primitives();
}

static struct Element *compile_exec_array(struct Token **tokens) {
    // Default capacity, expand when needed.
    int cap = 10;
    struct Element **elems = calloc(1, sizeof(struct Element) * cap);

    int len = 0;
    for (; tokens[tok_pos]->ty != TK_END_OF_FILE; tok_pos++) {
        struct Token *t = tokens[tok_pos];

        switch (t->ty) {
        case TK_NUMBER:
            elems[len++] = new_num_element(t->u.number);
            break;
        case TK_EXECUTABLE_NAME:
            elems[len++] = new_exec_name_element(t->u.name);
            break;
        case TK_LITERAL_NAME:
            elems[len++] = new_lit_name_element(t->u.name);
            break;
        case TK_OPEN_CURLY:
            tok_pos++;
            elems[len++] = compile_exec_array(tokens);
            break;
        case TK_CLOSE_CURLY:
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

static void eval_exec_name(const char *name) {
    struct Element *e = dict_get(name);
    if (e == NULL) {
        fprintf(stderr, "Unbind variable: %s\n", name);
        abort();
    }

    if (e->ty == ELEM_EXECUTABLE_ARRAY)
        eval_exec_array(e->u.byte_code);
    else
        eval_elem(e);
}

static void eval_elem(struct Element *e) {
    switch (e->ty) {
    case ELEM_NUMBER:
    case ELEM_LITERAL_NAME:
    case ELEM_EXECUTABLE_ARRAY:
        stack_push(e);
        break;
    case ELEM_EXECUTABLE_NAME:
        eval_exec_name(e->u.name);
        break;
    case ELEM_C_FUNC:
        e->u.cfunc();
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
    _init();

    for (; tokens[tok_pos]->ty != TK_END_OF_FILE; tok_pos++) {
        struct Token *t = tokens[tok_pos];
        switch (t->ty) {
        case TK_NUMBER:
            stack_push(new_num_element(t->u.number));
            break;
        case TK_EXECUTABLE_NAME:
            eval_exec_name(t->u.name);
            break;
        case TK_LITERAL_NAME:
            stack_push(new_lit_name_element(t->u.name));
            break;
        case TK_OPEN_CURLY:
            tok_pos++;
            stack_push(compile_exec_array(tokens));
            break;
        default:
            break;
        }
    }

    // if (!stack_is_empty())
    //     stack_print_all();
}

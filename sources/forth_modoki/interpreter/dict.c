#include "clesson.h"

static int dict_pos = 0;
static struct KeyValue dict_array[1024];

void dict_put(char *key, struct Token *elem) {
    for (int i = 0; i < dict_pos; i++) {
        if (streq(key, dict_array[i].key)) {
            // Already key exists, so we update value.
            dict_array[i].val = elem;
            return;
        }
    }

    dict_array[dict_pos].key = key;
    dict_array[dict_pos].val = elem;
    dict_pos++;
}

int dict_get(char *key, struct Token *out_elem) {
    for (int i = 0; i < dict_pos; i++) {
        if (streq(key, dict_array[i].key)) {
            *out_elem = *dict_array[i].val;
            return 1;
        }
    }
    return 0;
}

void dict_print_all(void) {
    for (int i = 0; i < dict_pos; i++) {
        printf("key: %s", dict_array[i].key);
        printf("val: ");
        token_print(dict_array[i].val);
    }
}

static void test_dict_get_no_exist(void) {
    struct Token t;
    int res = dict_get("nothing", &t);

    assert(res == 0);
}

static void test_dict_put_get(void) {
    struct Token *t_in = new_num_token(10);
    dict_put("foo", t_in);

    struct Token t_out;
    int res = dict_get("foo", &t_out);

    assert(res == 1);
    assert(t_out.u.number == 10);
}

static void test_dict_val_update(void) {
    struct Token *t1 = new_num_token(10);
    struct Token *t2 = new_num_token(20);

    dict_put("foo", t1);
    dict_put("foo", t2);

    struct Token t_out;
    int res = dict_get("foo", &t_out);

    assert(res == 1);
    assert(t_out.u.number == 20);
}

int main() {
    test_dict_get_no_exist();
    test_dict_put_get();
    test_dict_val_update();
}
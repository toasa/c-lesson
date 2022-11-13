#ifndef _DISASM_H_
#define _DISASM_H_

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// disasm.c
//
void test_disasm(void);

//
// cl_utils.c
//

void cl_clear_output();
char *cl_get_result(int num);
char *cl_get_all_result();
void cl_enable_buffer_mode();
void cl_disable_buffer_mode();
void cl_printf(char *fmt, ...);

//
// utils.c
//

bool streq(const char *s1, const char *s2);
void assert_int_eq(int x, int y);
void assert_str_eq(const char *s1, const char *s2);
void expect(bool b, const char *fmt, ...);

#endif
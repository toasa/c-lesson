#include <stdio.h>

int sum_range(int beg, int end) {
    int sum = 0;
    for (int i = beg; i <= end; i++) {
        sum += i;
    }
    return sum;
}

int sum_range_inline(int beg, int end) {
    int res;
    /*
     * args
     *   r0: beg
     *   r1: end
     *
     * Use r2 as loop counter.
     * Use r3 to store temp result.
     */
    asm("mov r2, r0");
    asm("mov r3, #0");

    asm("loop:");
    asm("cmp r2, r1");
    asm("bgt end");

    asm("add r3, r3, r2"); // sum+=i
    asm("add r2, r2, #1"); // i++
    asm("b loop");

    asm("end:");
    asm("mov %0, r3" : "=r"(res));

    return res;
}

/*
test code
*/

void assert_true(int boolflag) {
    if (!boolflag) {
        printf("assert fail\n");
    }
}

int main() {
    int res;

    res = sum_range(3, 10);
    assert_true(res == 52);

    res = sum_range_inline(3, 10);
    assert_true(res == 52);

    return 0;
}
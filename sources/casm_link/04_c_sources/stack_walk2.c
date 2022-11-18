#include <stdio.h>

void print_address(int address) { printf("  address: %x\n", address); }

void print_msg(char *str) { printf("  We get (%s)\n", str); }

#define FUNC2_SP (*((int *)(((int)&target) + 4)))
#define FUNC2_MSG_ADDR (*(int *)(FUNC2_SP)-20)
#define FUNC1_SP (*(int *)(FUNC2_MSG_ADDR + 4))
#define FUNC1_MSG_ADDR (*(int *)(FUNC1_SP)-20)
#define MAIN_SP (*(int *)(FUNC1_MSG_ADDR + 4))

void func3() {
    char *target;

    printf("We are in func3\n");
    print_msg(*(int *)(MAIN_SP - 8));
}

void func2() {
    char *msg = "func2 message.";
    printf("We are in func2, %s\n", msg);
    print_address((int)&msg);
    func3();
}

void func1() {
    char *msg = "func1 msg";
    printf("We are in func1, %s\n", msg);
    print_address((int)&msg);
    func2();
}

int main() {
    char *main_msg = "main msg.";
    printf("We are in main, %s\n", main_msg);
    print_address((int)&main_msg);
    func1();
    return 0;
}

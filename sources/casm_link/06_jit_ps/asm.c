#include "asm.h"

int asm_mov(int r1, int r2) { return 0xE1A00000 | (r1 << 12) | r2; }

int asm_mov_imm(int r, int imm) { return 0xE3A00000 | (r << 12) | imm; }

int asm_stfmd(int r) { return 0xE92D0000 | (1 << r); }

int asm_ldmia(int r) { return 0xE8BD0000 | (1 << r); }

int asm_add(int r, int r1, int r2) {
    return 0xE0800000 | (r << 12) | (r1 << 16) | r2;
}
enum Register {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    SP,
    LR,
    PC,
};

int asm_mov(int r1, int r2);
int asm_mov_imm(int r, int imm);
int asm_stfmd(int r);
int asm_ldmia(int r);
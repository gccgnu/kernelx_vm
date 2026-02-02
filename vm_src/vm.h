#ifndef VM_H
#define VM_H

/* Including libraries */
#include <stdio.h>
#include <stdlib.h>
#include "../type.h"
#include "stack.h"

/* Opcodes */
#define OP_PUSH  0x01
#define OP_ADD   0x02
#define OP_SUB   0x03
#define OP_MUL   0x04
#define OP_DIV   0x05
#define OP_ECALL 0x06
#define OP_JMP   0x07
#define OP_HALT  0xFF

/* Function prototypes */
void vm_run(u8* bytecode, u32 size);

#endif /* VM_H */
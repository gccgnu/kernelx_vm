#ifndef STACK_H
#define STACK_H

/* Including libraries */
#include <stdio.h>
#include <stdlib.h>
#include "../type.h"

/* Struct of stack */
typedef struct {
	i32* stack;
	u32 capacity;
	u32 count;
} Stack;

/* Function prototypes */
int stack_init(Stack* stack, u32 size);
int stack_remove(Stack* stack);

int stack_push(Stack* stack, u32 value);
int stack_pop(Stack* stack, int* value);
int stack_peek(Stack* stack, int* value);

int stack_dump(Stack* stack); // Debug function

#endif /* STACK_H */
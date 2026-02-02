/* Including header file */
#include "stack.h"

/* Function for init stack */
int
stack_init(Stack* stack, u32 size)
{
	stack->stack = (i32*)malloc(sizeof(i32) * size);
	if (stack == NULL) return 1;
	stack->count = 0;
	stack->capacity = size;
	return 0;
}

/* Function for remove stack */
int
stack_remove(Stack* stack)
{
	free(stack->stack);
	stack->stack = NULL;
	stack->count = 0;
	return 0;
}

/* Function for add to stack */
int
stack_push(Stack* stack, u32 value)
{
	if (stack->count >= stack->capacity) return 1;
	stack->stack[stack->count++] = value;
	return 0;
}

/* Function for get value from stack */
int  
stack_pop(Stack* stack, int* value)
{
	if (stack->count <= 0) return 1;
	*value = stack->stack[--stack->count];
	return 0;
}

/* Function for getting a value from the top of the stack without removing the value itself */
int 
stack_peek(Stack* stack, int* value)
{
	if (stack->count <= 0) return 1;
	*value = stack->stack[stack->count - 1];
	return 0;
}

/* DEBUG: Full stack dump */
int
stack_dump(Stack* stack)
{
	if (stack->count <= 0) {
		printf("Error: stack is empty!\n");
		return 1;
	}
	printf("Stack contents:\n");
	for (int i = 0; i < stack->count; i++) {
		printf(" Stack[%d] = %d\n", i, stack->stack[i]);
	}
	return 0;
}
/* Including libraries */
#include "vm.h"

/* Get argument from bytecode */
i32 
get_argument(u8* bytecode, u32 ip)
{
	i32 result = 0;
	result |= bytecode[ip + 0];
	result |= bytecode[ip + 1] << 8;
	result |= bytecode[ip + 2] << 16;
	result |= bytecode[ip + 3] << 24;
	return result;
}

/* VM execution */
void
vm_run(u8* bytecode, u32 size)
{
	/* Setup a stack */
	Stack stack;
	stack_init(&stack, 256);

	u32 ip = 0;

	/* Main loop */
	while (ip < size) {
		u8 opcode = bytecode[ip++];

		switch (opcode) {
			case OP_PUSH: {
				i32 value = get_argument(bytecode, ip);
				ip += 4;
				stack_push(&stack, value);
				// printf("PUSH %d\n", value);
				break;		
			}
			case OP_ADD: {
				i32 a,b;
				stack_pop(&stack, &b);
				stack_pop(&stack, &a);
				i32 result = a+b;
				stack_push(&stack, result);
				// printf("ADD %d + %d = %d\n", a, b, result);
				break;
			}
			case OP_SUB: {
				i32 a,b;
				stack_pop(&stack, &b);
				stack_pop(&stack, &a);
				i32 result = a-b;
				stack_push(&stack, result);
				// printf("SUB %d - %d = %d\n", a, b, result);
				break;
			}
			case OP_MUL: {
				i32 a,b;
				stack_pop(&stack, &b);
				stack_pop(&stack, &a);
				i32 result = a*b;
				stack_push(&stack, result);
				// printf("MUL %d * %d = %d\n", a, b, result);
				break;
			}
			case OP_DIV: {
				i32 a,b;
				stack_pop(&stack, &b);
				stack_pop(&stack, &a);

				if (b == 0) {
					printf("Error! Division by zero!\n");
					stack_remove(&stack);
					return;
				}

				i32 result = a/b;
				stack_push(&stack, result);
				// printf("DIV %d / %d = %d\n", a, b, result);
				break;
			}
			case OP_ECALL: {
				i32 value = get_argument(bytecode, ip);
				ip += 4;
				// printf("ECALL %d\n", value);
				switch (value) {
					case 10: {
						i32 value;
						stack_pop(&stack, &value);
						printf("%d\n", value);
						break;
					}
					default: {
						printf("Error! Unknown enviroment call\n");
						break;
					}
				}
				break;
			}
			case OP_JMP: {
				i32 value = get_argument(bytecode, ip); 
				ip = (u32)value;
				// printf("JMP %d (ip=%d)\n", value, ip);
				break;
			}
			case OP_HALT: {
				stack_remove(&stack);
				// printf("HALT\n");
				return;
			}
			default: {
				printf("Unknown opcode: 0x%02X\n", opcode);
				stack_remove(&stack);
				return;
			}
		}
	}

	stack_remove(&stack);
	/* Finished */
}
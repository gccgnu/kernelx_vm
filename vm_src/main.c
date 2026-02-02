#include <stdio.h>
#include "stack.h"
#include "vm.h"

int
main (int argc, char* argv[]) 
{
	/* Check args */
	if (argc != 2) {
		printf("Usage: %s <program>\n", argv[0]);
		return 1;
	}

	/* Open file */
	FILE* file = fopen(argv[1], "rb");
	if (!file) {
		printf("Error: cannot open file '%s'\n", argv[1]);
		return 1;
	}

	/* Getting program size */
	fseek(file, 0, SEEK_END);
	u32 program_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	/* Allocating memory for program */
	u8* program = malloc(program_size);
	if (!program) {
		printf("Error: cannot allocate memory\n");
		fclose(file);
		return 1;
	}

	/* Reading all bytes */
	size_t bytes_read = fread(program, 1, program_size, file);
	if (bytes_read != program_size) {
		printf("Error failed to read file (read %zu of %u bytes)\n",
			bytes_read, program_size);
		free(program);
		fclose(file);
	}

	/* Printing information */
	printf("Program loaded to memory, program size: %d! executing...\n", program_size);

	/* Run app */
	vm_run(program, program_size);

	/* Clearing */
	free(program);
	fclose(file);

	return 0;
}
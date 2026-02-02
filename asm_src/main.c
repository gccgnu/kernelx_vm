/* Including libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Opcodes definition */
#define OP_PUSH  0x01
#define OP_ADD   0x02
#define OP_SUB   0x03
#define OP_MUL   0x04
#define OP_DIV   0x05
#define OP_ECALL 0x06
#define OP_JMP   0x07
#define OP_HALT  0xFF

/* Dynamic labels storage */
char** label_names;
int* label_addrs;
int label_cnt, label_cap;

/* Dynamic bytecode buffer */
unsigned char* code;
int code_sz, code_cap;

/* Initialize dynamic arrays */
void 
init() 
{
	label_cap = 8;
	label_names = malloc(label_cap * sizeof(char*));
	label_addrs = malloc(label_cap * sizeof(int));
	label_cnt = 0;
	
	code_cap = 64;
	code = malloc(code_cap);
	code_sz = 0;
}

/* Free all allocated memory */
void 
cleanup()
{
	for (int i = 0; i < label_cnt; i++) 
		free(label_names[i]);
	free(label_names);
	free(label_addrs);
	free(code);
}

/* Add label to the table */
void 
add_label(char* name, int addr)
{
	/* Grow array if needed */
	if (label_cnt >= label_cap) {
		label_cap *= 2;
		label_names = realloc(label_names, label_cap * sizeof(char*));
		label_addrs = realloc(label_addrs, label_cap * sizeof(int));
	}
	
	label_names[label_cnt] = strdup(name);
	label_addrs[label_cnt++] = addr;
}

/* Find label address by name */
int
find_label(char* name)
{
	for (int i = 0; i < label_cnt; i++)
		if (!strcmp(label_names[i], name)) 
			return label_addrs[i];
	
	fprintf(stderr, "Error: undefined label '%s'\n", name);
	exit(1);
}

/* Emit single byte to bytecode */
void 
emit(unsigned char b)
{
	/* Grow buffer if needed */
	if (code_sz >= code_cap) {
		code_cap *= 2;
		code = realloc(code, code_cap);
	}
	
	code[code_sz++] = b;
}

/* Emit 32-bit integer (little-endian) */
void 
emit32(int v)
{
	emit(v);
	emit(v >> 8);
	emit(v >> 16);
	emit(v >> 24);
}

/* Get instruction size in bytes */
int 
op_size(char* op)
{
	if (!strcmp(op, "push") || !strcmp(op, "jmp") || !strcmp(op, "ecall")) 
		return 5;
	return 1;
}

/* Parse and process single line */
void 
parse_line(char* line, int pass, int* addr)
{
	/* Remove comments */
	char* p = strchr(line, ';'); 
	if (p) *p = 0;
	
	/* Trim leading whitespace */
	while (isspace(*line)) 
		line++;
	
	/* Trim trailing whitespace */
	int len = strlen(line);
	while (len > 0 && isspace(line[len-1])) 
		line[--len] = 0;
	
	/* Skip empty lines */
	if (!len) return;
	
	/* Handle labels */
	if (line[len-1] == ':') {
		line[len-1] = 0;
		if (pass == 1) 
			add_label(line, *addr);
		return;
	}
	
	/* Parse instruction and argument */
	char op[32], arg[32] = {0};
	sscanf(line, "%s %s", op, arg);
	
	/* Pass 1: only calculate addresses */
	if (pass == 1) { 
		*addr += op_size(op); 
		return; 
	}
	
	/* Pass 2: generate bytecode */
	if (!strcmp(op, "push")) { 
		emit(OP_PUSH); 
		emit32(atoi(arg)); 
	}
	else if (!strcmp(op, "add"))   emit(OP_ADD);
	else if (!strcmp(op, "sub"))   emit(OP_SUB);
	else if (!strcmp(op, "mul"))   emit(OP_MUL);
	else if (!strcmp(op, "div"))   emit(OP_DIV);
	else if (!strcmp(op, "halt"))  emit(OP_HALT);
	else if (!strcmp(op, "ecall")) { 
		emit(OP_ECALL); 
		emit32(atoi(arg)); 
	}
	else if (!strcmp(op, "jmp")) { 
		emit(OP_JMP); 
		emit32(find_label(arg)); 
	}
	else { 
		fprintf(stderr, "Error: unknown instruction '%s'\n", op); 
		exit(1); 
	}
}

/* Two-pass assembler */
void 
assemble(char* src) 
{
	/* Pass 1: collect labels */
	/* Pass 2: generate code */
	for (int pass = 1; pass <= 2; pass++) {
		int addr = 0;
		char* copy = strdup(src);
		
		for (char* line = strtok(copy, "\n"); line; line = strtok(NULL, "\n"))
			parse_line(line, pass, &addr);
		
		free(copy);
	}
}

/* Entry point */
int 
main(int argc, char** argv)
{
	if (argc < 3) {
		printf("Usage: %s <input.asm> <output.bin>\n", argv[0]);
		return 1;
	}
	
	/* Open source file */
	FILE* f = fopen(argv[1], "r");
	if (!f) { 
		fprintf(stderr, "Error: cannot open '%s'\n", argv[1]); 
		return 1; 
	}

	/* Read entire file */
	fseek(f, 0, SEEK_END); 
	long sz = ftell(f); 
	fseek(f, 0, SEEK_SET);
	
	char* src = malloc(sz + 1);
	fread(src, 1, sz, f);
	src[sz] = 0;
	fclose(f);
	
	/* Run assembler */
	init();
	assemble(src);
	
	/* Write output binary */
	f = fopen(argv[2], "wb");
	fwrite(code, 1, code_sz, f);
	fclose(f);
	
	printf("OK: %d bytes -> %s\n", code_sz, argv[2]);
	
	/* Cleanup */
	free(src);
	cleanup();
	
	return 0;
}
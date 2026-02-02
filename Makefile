CC = gcc
CFLAGS = -Wall -O2 -march=native

all: asm vm

asm: $(wildcard asm_src/*.c)
	$(CC) $(CFLAGS) $^ -o $@
	strip $@

vm: $(wildcard vm_src/*.c)
	$(CC) $(CFLAGS) $^ -o $@
	strip $@

clean:
	rm -f asm vm

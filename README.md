# Simple VM

Simple vm for any task (for embedded systems, for operating systems, etc.), written in C, with a simple assembler included

## **Build in Linux**
```BASH
make
```

## Instruction Set

| Opcode | Instruction | Description |
|--------|-------------|-------------|
| 0x01 | `push <i32>` | Push value onto stack |
| 0x02 | `add` | Pop two values, push sum |
| 0x03 | `sub` | Pop two values, push difference |
| 0x04 | `mul` | Pop two values, push product |
| 0x05 | `div` | Pop two values, push quotient |
| 0x06 | `ecall <n>` | Environment call |
| 0x07 | `jmp <label>` | Unconditional jump |
| 0xFF | `halt` | Stop execution |

## Environment Calls

| Number | Description |
|--------|-------------|
| 10 | Print top of stack as integer |


## Example

```asm
; Calculate (5 + 3) * 2 = 16
push 5
push 3
add
push 2
mul
ecall 10    ; print result
halt
```

```bash
./asm test.asm test.bin
./vm test.bin
```

Output:
```
16
```

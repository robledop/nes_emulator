#pragma once

#include <stdbool.h>

#define MAX_MEMORY 65536

typedef unsigned char byte;
typedef  unsigned short word;
typedef enum address_mode
{
	implicit,
	accumulator,
	immediate,
	zero_page,
	zero_page_x,
	zero_page_y,
	relative,
	absolute,
	absolute_x,
	absolute_y,
	indirect,
	indexed_indirect,
	indirect_indexed
} address_mode;

typedef struct
{
	byte data[MAX_MEMORY];
}memory;

typedef struct
{
	// Accumulator
	byte A;

	// Index register
	byte X, Y;

	// Stack pointer
	byte SP;

	// Processor status
	// N, V, 1, B, D, I, Z, C
	// N = Negative
	// V = Overflow
	// 1 = Unused
	// B = BRK command 1 = BRK, 0 = IRQB
	// D = decimal mode
	// I = IRQB disable
	// Z = Zero
	// C = Carry
	byte P;

	// Program counter
	word PC;

	memory memory;
} cpu;

#define OP(opcode, operation, address_mode) \
case 0x##opcode: \
	##operation(cpu, ##address_mode); \
	break;

void cpu_exec(cpu* cpu, byte instruction);
void cpu_clear_memory(cpu* cpu);
void cpu_init(cpu* cpu);

bool cpu_get_C_flag(const cpu* cpu);
bool cpu_get_Z_flag(const cpu* cpu);
bool cpu_get_I_flag(const cpu* cpu);
bool cpu_get_D_flag(const cpu* cpu);
bool cpu_get_B_flag(const cpu* cpu);
bool cpu_get_V_flag(const cpu* cpu);
bool cpu_get_N_flag(const cpu* cpu);

void cpu_set_C_flag(cpu* cpu, const char val);
void cpu_set_Z_flag(cpu* cpu, const char val);
void cpu_set_I_flag(cpu* cpu, const char val);
void cpu_set_D_flag(cpu* cpu, const char val);
void cpu_set_B_flag(cpu* cpu, const char val);
void cpu_set_V_flag(cpu* cpu, const char val);
void cpu_set_N_flag(cpu* cpu, const char val);


// opcodes

void lda(cpu* cpu, address_mode address_mode);
void ldx(cpu* cpu, const address_mode address_mode);
void ldy(cpu* cpu, const address_mode address_mode);
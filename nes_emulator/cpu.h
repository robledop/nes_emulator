#pragma once

#include <stdbool.h>

#define MAX_MEMORY 65536
#define STACK_BASE 0x100

#define CARRY_FLAG     0x01
#define ZERO_FLAG      0x02
#define INTERRUPT_FLAG 0x04
#define DECIMAL_FLAG   0x08
#define BREAK_FLAG     0x10
#define CONSTANT_FLAG  0x20
#define OVERFLOW_FLAG  0x40
#define SIGN_FLAG      0x80

#define SIGN_BIT     0x80

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
	byte a;

	// Index register
	byte x, y;

	// Stack pointer
	byte sp;

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
	byte p;

	// Program counter
	word pc;

	memory memory;
} cpu;

#define OP(opcode, operation, address_mode) \
case 0x##opcode: \
	##operation(cpu, ##address_mode); \
	break

void cpu_exec(cpu* cpu, byte instruction);
void cpu_clear_memory(cpu* cpu);
void cpu_init(cpu* cpu);

bool cpu_get_c_flag(const cpu* cpu);
bool cpu_get_z_flag(const cpu* cpu);
bool cpu_get_i_flag(const cpu* cpu);
bool cpu_get_d_flag(const cpu* cpu);
bool cpu_get_b_flag(const cpu* cpu);
bool cpu_get_v_flag(const cpu* cpu);
bool cpu_get_n_flag(const cpu* cpu);

void cpu_set_c_flag(cpu* cpu, const unsigned char val);
void cpu_set_z_flag(cpu* cpu, const unsigned char val);
void cpu_set_i_flag(cpu* cpu, const char val);
void cpu_set_d_flag(cpu* cpu, const char val);
void cpu_set_b_flag(cpu* cpu, const char val);
void cpu_set_v_flag(cpu* cpu, const char val);
void cpu_set_n_flag(cpu* cpu, const char val);
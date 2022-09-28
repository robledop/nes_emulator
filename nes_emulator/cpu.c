#include "cpu.h"

#include <stdio.h>

#include "memory.h"

// https://www.nesdev.org/obelisk-6502-guide/reference.html

static void handle_lda_flags(cpu* cpu)
{
	if (cpu->A & 0b10000000)
	{
		cpu_set_N_flag(cpu, 1);
	}

	if (cpu->A == 0)
	{
		cpu_set_Z_flag(cpu, 1);
	}
}

void cpu_exec(cpu* cpu, const byte instruction)
{
	switch (instruction)
	{
		// LDA immediate
		// LDA #10         ;Load 10 ($0A) into the accumulator
		// https://www.nesdev.org/obelisk-6502-guide/addressing.html#IMM
		case 0xA9:
			cpu->PC++;
			cpu->A = cpu->memory.data[cpu->PC];
			handle_lda_flags(cpu);
			break;

			// LDA Zero Page
			// LDA $00         ;Load accumulator from $00
			// https://www.nesdev.org/obelisk-6502-guide/addressing.html#ZPG
		case 0xA5:
			cpu->PC++;
			cpu->A = cpu->memory.data[cpu->memory.data[cpu->PC]];
			handle_lda_flags(cpu);
			break;

			// LDA Zero Page,X
			// LDA $10,X       ;Load accumulator from location 10 + X
			// https://www.nesdev.org/obelisk-6502-guide/addressing.html#ZPX
		case 0xB5:
		{
			cpu->PC++;
			const word address = cpu->memory.data[cpu->PC] + cpu->X;
			cpu->A = cpu->memory.data[address];
			handle_lda_flags(cpu);
		}
		break;

		// LDA Absolute
		// LDA $1234       ;Load accumulator from location $1234
		// https://www.nesdev.org/obelisk-6502-guide/addressing.html#ABS
		case 0xAD:
		{
			cpu->PC++;
			const byte low_byte = cpu->memory.data[cpu->PC];
			cpu->PC++;
			const byte high_byte = cpu->memory.data[cpu->PC];
			const word address = ((word)(high_byte << 8)) | low_byte;
			cpu->A = cpu->memory.data[address];
			handle_lda_flags(cpu);
		}
		break;

		// LDA Absolute,X
		case 0xBD:
			break;

			// LDA Absolute,Y
		case 0xB9:
			break;

			// LDA (Indirect,X)
		case 0xA1:

			break;

			// LDA (Indirect),Y
		case 0xB1:
			break;

		default:
			printf("%s", "Unsupported opcode");
	}
}

void cpu_clear_memory(cpu* cpu)
{
	memset(&cpu->memory, 0, MAX_MEMORY);
}

void cpu_init(cpu* cpu)
{
	cpu->SP = 0xFF;
	cpu->P = 0x00;
	cpu->A = 0x00;
	cpu->X = 0x00;
	cpu->Y = 0x00;
	cpu->PC = ((word)(cpu->memory.data[0xFFFD] << 8)) | cpu->memory.data[0xFFFC];
}

bool cpu_get_C_flag(const cpu* cpu)
{
	return cpu->P & 0b00000001;
}

bool cpu_get_Z_flag(const cpu* cpu)
{
	return cpu->P & 0b00000010;
}

bool cpu_get_I_flag(const cpu* cpu)
{
	return cpu->P & 0b00000100;
}

bool cpu_get_D_flag(const cpu* cpu)
{
	return cpu->P & 0b00001000;
}

bool cpu_get_B_flag(const cpu* cpu)
{
	return cpu->P & 0b00010000;
}

bool cpu_get_V_flag(const cpu* cpu)
{
	return cpu->P & 0b00100000;
}

bool cpu_get_N_flag(const cpu* cpu)
{
	return cpu->P & 0b10000000;
}


void cpu_set_C_flag(cpu* cpu, const char val)
{
	cpu->P ^= (-val ^ cpu->P) & (1UL << 0);
}

void cpu_set_Z_flag(cpu* cpu, const char val)
{
	cpu->P ^= (-val ^ cpu->P) & (1UL << 1);
}

void cpu_set_I_flag(cpu* cpu, const char val)
{
	cpu->P ^= (-val ^ cpu->P) & (1UL << 2);
}

void cpu_set_D_flag(cpu* cpu, const char val)
{
	cpu->P ^= (-val ^ cpu->P) & (1UL << 3);
}

void cpu_set_B_flag(cpu* cpu, const char val)
{
	cpu->P ^= (-val ^ cpu->P) & (1UL << 4);
}

void cpu_set_V_flag(cpu* cpu, const char val)
{
	cpu->P ^= (-val ^ cpu->P) & (1UL << 6);
}

void cpu_set_N_flag(cpu* cpu, const char val)
{
	cpu->P ^= (-val ^ cpu->P) & (1UL << 7);
}
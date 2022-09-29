#include "cpu.h"

#include <assert.h>
#include <stdio.h>
#include "memory.h"

// https://www.nesdev.org/obelisk-6502-guide/reference.html

static void set_negative_and_zero_flags(cpu* cpu, const byte reg)
{
	if (reg & 0b10000000)
	{
		cpu_set_N_flag(cpu, 1);
	}

	if (reg == 0)
	{
		cpu_set_Z_flag(cpu, 1);
	}
}

void cpu_exec(cpu* cpu, const byte instruction)
{
	switch (instruction)
	{
		OP(A9, lda, immediate)
			OP(A5, lda, zero_page)
			OP(B5, lda, zero_page_x)
			OP(AD, lda, absolute)
			OP(BD, lda, absolute_x)
			OP(B9, lda, absolute_y)
			OP(A1, lda, indexed_indirect)
			OP(B1, lda, indirect_indexed)

			OP(A2, ldx, immediate)
			OP(A6, ldx, zero_page)
			OP(B6, ldx, zero_page_y)
			OP(AE, ldx, absolute)
			OP(BE, ldx, absolute_y)

			OP(A0, ldy, immediate)
			OP(A4, ldy, zero_page)
			OP(B4, ldy, zero_page_x)
			OP(AC, ldy, absolute)
			OP(BC, ldy, absolute_x)

			///////////////////////////////////////////////////////////////////
			// ! ADC opcodes
			///////////////////////////////////////////////////////////////////

		case 0x69:
			break;

		case 0x65:
			break;

		case 0x75:
			break;

		case 0x6D:
			break;

		case 0x7D:
			break;
		case 0x79:
			break;
		case 0x61:
			break;
		case 0x71:
			break;

			///////////////////////////////////////////////////////////////////
			// ! AND opcodes
			///////////////////////////////////////////////////////////////////

		case 0x29:
			break;
		case 0x25:
			break;
		case 0x35:
			break;
		case 0x2D:
			break;
		case 0x3D:
			break;
		case 0x39:
			break;
		case 0x21:
			break;
		case 0x31:
			break;

			///////////////////////////////////////////////////////////////////
			// ! ASL opcodes
			///////////////////////////////////////////////////////////////////

		case 0x0A:
			break;
		case 0x06:
			break;
		case 0x16:
			break;
		case 0x0E:
			break;
		case 0x1E:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BCC opcodes
			///////////////////////////////////////////////////////////////////

		case 0x90:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BCS opcodes
			///////////////////////////////////////////////////////////////////

		case 0xB0:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BEQ opcodes
			///////////////////////////////////////////////////////////////////

		case 0xF0:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BIT opcodes
			///////////////////////////////////////////////////////////////////

		case 0x24:
			break;
		case 0x2C:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BMI opcodes
			///////////////////////////////////////////////////////////////////

		case 0x30:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BNE opcodes
			///////////////////////////////////////////////////////////////////

		case 0xD0:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BPL opcodes
			///////////////////////////////////////////////////////////////////

		case 0x10:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BRK opcodes
			///////////////////////////////////////////////////////////////////

		case 0x00:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BVC opcodes
			///////////////////////////////////////////////////////////////////

		case 0x50:
			break;

			///////////////////////////////////////////////////////////////////
			// ! BVS opcodes
			///////////////////////////////////////////////////////////////////

		case 0x70:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CLC opcodes
			///////////////////////////////////////////////////////////////////

		case 0x18:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CLD opcodes
			///////////////////////////////////////////////////////////////////

		case 0xd8:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CLI opcodes
			///////////////////////////////////////////////////////////////////

		case 0x58:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CLV opcodes
			///////////////////////////////////////////////////////////////////

		case 0xB8:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CMP opcodes
			///////////////////////////////////////////////////////////////////

		case 0xC9:
			break;
		case 0xC5:
			break;
		case 0xD5:
			break;
		case 0xCD:
			break;
		case 0xDD:
			break;
		case 0xD9:
			break;
		case 0xC1:
			break;
		case 0xD1:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CPX opcodes
			///////////////////////////////////////////////////////////////////

		case 0xE0:
			break;
		case 0xE4:
			break;
		case 0xEC:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CPY opcodes
			///////////////////////////////////////////////////////////////////

		case 0xC0:
			break;
		case 0xC4:
			break;
		case 0xCC:
			break;

			///////////////////////////////////////////////////////////////////
			// ! DEC opcodes
			///////////////////////////////////////////////////////////////////

		case 0xC6:
			break;
		case 0xD6:
			break;
		case 0xCE:
			break;
		case 0xDE:
			break;

			///////////////////////////////////////////////////////////////////
			// ! DEX opcodes
			///////////////////////////////////////////////////////////////////

		case 0xCA:
			break;

			///////////////////////////////////////////////////////////////////
			// ! CEY opcodes
			///////////////////////////////////////////////////////////////////

		case 0x88:
			break;

			///////////////////////////////////////////////////////////////////
			// ! EOR opcodes
			///////////////////////////////////////////////////////////////////

		case 0x49:
			break;
		case 0x45:
			break;
		case 0x55:
			break;
		case 0x4D:
			break;
		case 0x5D:
			break;
		case 0x59:
			break;
		case 0x41:
			break;
		case 0x51:
			break;

			///////////////////////////////////////////////////////////////////
			// ! INC opcodes
			///////////////////////////////////////////////////////////////////

		case 0xE6:
			break;
		case 0xF6:
			break;
		case 0xEE:
			break;
		case 0xFE:
			break;

			///////////////////////////////////////////////////////////////////
			// ! INX opcodes
			///////////////////////////////////////////////////////////////////

		case 0xE8:
			break;

			///////////////////////////////////////////////////////////////////
			// ! INY opcodes
			///////////////////////////////////////////////////////////////////

		case 0xC8:
			break;

			///////////////////////////////////////////////////////////////////
			// ! JMP opcodes
			///////////////////////////////////////////////////////////////////

		case 0x4C:
			break;
		case 0x6C:
			break;

			///////////////////////////////////////////////////////////////////
			// ! JSR opcodes
			///////////////////////////////////////////////////////////////////

		case 0x20:
			break;

			///////////////////////////////////////////////////////////////////
			// ! LSR opcodes
			///////////////////////////////////////////////////////////////////

		case 0x4A:
			break;
		case 0x46:
			break;
		case 0x56:
			break;
		case 0x4E:
			break;
		case 0x5E:
			break;

			///////////////////////////////////////////////////////////////////
			// ! NOP opcodes
			///////////////////////////////////////////////////////////////////

		case 0xEA:
			break;

			///////////////////////////////////////////////////////////////////
			// ! ORA opcodes
			///////////////////////////////////////////////////////////////////

		case 0x09:
			break;
		case 0x05:
			break;
		case 0x15:
			break;
		case 0x0D:
			break;
		case 0x1D:
			break;
		case 0x19:
			break;
		case 0x01:
			break;
		case 0x11:
			break;

			///////////////////////////////////////////////////////////////////
			// ! PHA opcodes
			///////////////////////////////////////////////////////////////////

		case 0x48:
			break;

			///////////////////////////////////////////////////////////////////
			// ! PHP opcodes
			///////////////////////////////////////////////////////////////////

		case 0x08:
			break;

			///////////////////////////////////////////////////////////////////
			// ! PLA opcodes
			///////////////////////////////////////////////////////////////////

		case 0x68:
			break;

			///////////////////////////////////////////////////////////////////
			// ! PLP opcodes
			///////////////////////////////////////////////////////////////////

		case 0x28:
			break;

			///////////////////////////////////////////////////////////////////
			// ! ROL opcodes
			///////////////////////////////////////////////////////////////////

		case 0x2A:
			break;
		case 0x26:
			break;
		case 0x36:
			break;
		case 0x2E:
			break;
		case 0x3E:
			break;

			///////////////////////////////////////////////////////////////////
			// ! ROR opcodes
			///////////////////////////////////////////////////////////////////

		case 0x6A:
			break;
		case 0x66:
			break;
		case 0x76:
			break;
		case 0x6E:
			break;
		case 0x7E:
			break;

			///////////////////////////////////////////////////////////////////
			// ! RTI opcodes
			///////////////////////////////////////////////////////////////////

		case 0x40:
			break;

			///////////////////////////////////////////////////////////////////
			// ! RTS opcodes
			///////////////////////////////////////////////////////////////////

		case 0x60:
			break;

			///////////////////////////////////////////////////////////////////
			// ! SBC opcodes
			///////////////////////////////////////////////////////////////////

		case 0xE9:
			break;
		case 0xE5:
			break;
		case 0xF5:
			break;
		case 0xED:
			break;
		case 0xFD:
			break;
		case 0xF9:
			break;
		case 0xE1:
			break;
		case 0xF1:
			break;

			///////////////////////////////////////////////////////////////////
			// ! SEC opcodes
			///////////////////////////////////////////////////////////////////

		case 0x38:
			break;

			///////////////////////////////////////////////////////////////////
			// ! SED opcodes
			///////////////////////////////////////////////////////////////////

		case 0xF8:
			break;

			///////////////////////////////////////////////////////////////////
			// ! SEI opcodes
			///////////////////////////////////////////////////////////////////

		case 0x78:
			break;

			///////////////////////////////////////////////////////////////////
			// ! STA opcodes
			///////////////////////////////////////////////////////////////////

		case 0x85:
			break;
		case 0x95:
			break;
		case 0x8D:
			break;
		case 0x9D:
			break;
		case 0x99:
			break;
		case 0x81:
			break;
		case 0x91:
			break;

			///////////////////////////////////////////////////////////////////
			// ! STX opcodes
			///////////////////////////////////////////////////////////////////

		case 0x86:
			break;
		case 0x96:
			break;
		case 0x8E:
			break;

			///////////////////////////////////////////////////////////////////
			// ! STY opcodes
			///////////////////////////////////////////////////////////////////

		case 0x84:
			break;
		case 0x94:
			break;
		case 0x8C:

			///////////////////////////////////////////////////////////////////
			// ! TAX opcodes
			///////////////////////////////////////////////////////////////////

		case 0xAA:
			break;

			///////////////////////////////////////////////////////////////////
			// ! TAY opcodes
			///////////////////////////////////////////////////////////////////

		case 0xA8:
			break;

			///////////////////////////////////////////////////////////////////
			// ! TSX opcodes
			///////////////////////////////////////////////////////////////////

		case 0xBA:
			break;

			///////////////////////////////////////////////////////////////////
			// ! TXA opcodes
			///////////////////////////////////////////////////////////////////

		case 0x8A:
			break;

			///////////////////////////////////////////////////////////////////
			// ! TXS opcodes
			///////////////////////////////////////////////////////////////////

		case 0x9A:
			break;


			///////////////////////////////////////////////////////////////////
			// ! TYA opcodes
			///////////////////////////////////////////////////////////////////

		case 0x98:
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

static word get_memory_address(cpu* cpu, const address_mode address_mode)
{
	switch (address_mode) {
		case implicit: break;
		case accumulator: break;
		case immediate: return cpu->PC;
		case zero_page: return cpu->memory.data[cpu->PC];
		case zero_page_x: return cpu->memory.data[cpu->PC] + cpu->X;
		case zero_page_y: break;
		case relative: break;
		case absolute:
		{
			const byte low_byte = cpu->memory.data[cpu->PC];
			cpu->PC++;
			const byte high_byte = cpu->memory.data[cpu->PC];
			return  ((word)(high_byte << 8)) | low_byte;
		}
		case absolute_x:
		{
			const byte low_byte = cpu->memory.data[cpu->PC];
			cpu->PC++;
			const byte high_byte = cpu->memory.data[cpu->PC];
			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->X;
			return address;
		}
		case absolute_y:
		{
			const byte low_byte = cpu->memory.data[cpu->PC];
			cpu->PC++;
			const byte high_byte = cpu->memory.data[cpu->PC];
			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->Y;
			return address;
		}
		case indirect: break;
		case indexed_indirect:
		{
			const byte vector = cpu->memory.data[cpu->PC] + cpu->X;

			const byte low_byte = cpu->memory.data[vector];
			const byte high_byte = cpu->memory.data[vector + 1];
			return  ((word)(high_byte << 8)) | low_byte;
		}
		break;
		case indirect_indexed:
		{
			const byte vector = cpu->memory.data[cpu->PC];

			const byte low_byte = cpu->memory.data[vector];
			const byte high_byte = cpu->memory.data[vector + 1];

			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->Y;
			return  address;
		}
		break;
		default: assert(false);
	}
}

void lda(cpu* cpu, const address_mode address_mode)
{
	cpu->PC++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->A = cpu->memory.data[address];

	set_negative_and_zero_flags(cpu, cpu->A);
}

void ldx(cpu* cpu, const address_mode address_mode)
{

	return;
}

void ldy(cpu* cpu, const address_mode address_mode)
{

	return;
}
#include "cpu.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "memory.h"


static void calc_carry(cpu* cpu, const word byte)
{
	if (byte & 0xFF00) {
		cpu_set_c_flag(cpu, 1);
	}
	else {
		cpu_set_c_flag(cpu, 0);
	}
}

static void calc_zero(cpu* cpu, const byte byte)
{
	if (byte == 0x00) {
		cpu_set_z_flag(cpu, 1);
	}
	else {
		cpu_set_z_flag(cpu, 0);
	}
}

static void calc_negative(cpu* cpu, const byte byte)
{
	if (byte & 0b10000000) {
		cpu_set_n_flag(cpu, 1);
	}
	else {
		cpu_set_n_flag(cpu, 0);
	}
}

// http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
// Two numbers that have the same sign are added, and the result has a different sign
static void calc_overflow(cpu* cpu, const word result, const byte operand)
{
	const byte a_sign = cpu->a & 0x80;
	const byte operand_sign = operand & 0x80;
	const byte result_sign = result & 0x80;

	if ((a_sign == operand_sign) && a_sign != result_sign)
	{
		cpu_set_v_flag(cpu, 1);
	}
	else
	{
		cpu_set_v_flag(cpu, 0);
	}
}

static word get_memory_address(cpu* cpu, const address_mode address_mode)
{
	switch (address_mode) {
		case implicit:
			assert(false);
			break;

		case accumulator:
			break;

		case immediate:
			return cpu->pc;

		case zero_page:
			return cpu->memory.data[cpu->pc];

		case zero_page_x:
			return cpu->memory.data[cpu->pc] + cpu->x;

		case zero_page_y:
			return cpu->memory.data[cpu->pc] + cpu->y;

		case relative:
			return cpu->memory.data[cpu->pc];

		case absolute:
		{
			const byte low_byte = cpu->memory.data[cpu->pc];
			cpu->pc++;
			const byte high_byte = cpu->memory.data[cpu->pc];
			return  ((word)(high_byte << 8)) | low_byte;
		}

		case absolute_x:
		{
			const byte low_byte = cpu->memory.data[cpu->pc];
			cpu->pc++;
			const byte high_byte = cpu->memory.data[cpu->pc];
			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->x;
			return address;
		}

		case absolute_y:
		{
			const byte low_byte = cpu->memory.data[cpu->pc];
			cpu->pc++;
			const byte high_byte = cpu->memory.data[cpu->pc];
			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->y;
			return address;
		}

		case indirect:
			assert(false); break;

		case indexed_indirect:
		{
			const byte vector = cpu->memory.data[cpu->pc] + cpu->x;

			const byte low_byte = cpu->memory.data[vector];
			const byte high_byte = cpu->memory.data[vector + 1];
			return  ((word)(high_byte << 8)) | low_byte;
		}

		case indirect_indexed:
		{
			const byte vector = cpu->memory.data[cpu->pc];

			const byte low_byte = cpu->memory.data[vector];
			const byte high_byte = cpu->memory.data[vector + 1];

			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->y;
			return  address;
		}

		default: assert(false);
	}

	return 0;
}


static void lda(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->a = cpu->memory.data[address];

	calc_zero(cpu, cpu->a);
	calc_negative(cpu, cpu->a);
}

static void ldx(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->x = cpu->memory.data[address];

	calc_zero(cpu, cpu->x);
	calc_negative(cpu, cpu->x);
}

static void ldy(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->y = cpu->memory.data[address];

	calc_zero(cpu, cpu->y);
	calc_negative(cpu, cpu->y);
}

// Add with Carry
static void adc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = cpu->memory.data[address];
	const word result = cpu->a + memory + (cpu_get_v_flag(cpu) ? 1 : 0);
	calc_carry(cpu, result);
	calc_overflow(cpu, result, memory);
	calc_zero(cpu, (byte)result);
	calc_negative(cpu, (byte)result);
	cpu->a = (byte)result;
}

// Logical AND
static void and (cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->a &= cpu->memory.data[address];
	calc_negative(cpu, cpu->a);
	calc_zero(cpu, cpu->a);
}

// Arithmetic Shift Left
static void asl(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0x10000000));
		cpu->a <<= 1;
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		cpu_set_c_flag(cpu, (cpu->memory.data[address] & 0x10000000));
		cpu->memory.data[address] <<= 1;
	}
}

// Branch if Carry Clear
static void bcc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_c_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// Branch if Carry Set
static void bcs(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_c_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// Branch if Equal
static void beq(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_z_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// Bit Test
static void bit(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte result = cpu->a & cpu->memory.data[address];
	cpu_set_z_flag(cpu, result);

	cpu->p = cpu->memory.data[address] & 0b11000000;
}

// Branch if Minus
static void bmi(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_n_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// Branch if Not Equal
static void bne(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_z_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// Branch if Positive
static void bpl(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_n_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// https://www.nesdev.org/obelisk-6502-guide/reference.html
void cpu_exec(cpu* cpu, const byte instruction)
{
	switch (instruction)
	{
		OP(A9, lda, immediate);
		OP(A5, lda, zero_page);
		OP(B5, lda, zero_page_x);
		OP(AD, lda, absolute);
		OP(BD, lda, absolute_x);
		OP(B9, lda, absolute_y);
		OP(A1, lda, indexed_indirect);
		OP(B1, lda, indirect_indexed);

		OP(A2, ldx, immediate);
		OP(A6, ldx, zero_page);
		OP(B6, ldx, zero_page_y);
		OP(AE, ldx, absolute);
		OP(BE, ldx, absolute_y);

		OP(A0, ldy, immediate);
		OP(A4, ldy, zero_page);
		OP(B4, ldy, zero_page_x);
		OP(AC, ldy, absolute);
		OP(BC, ldy, absolute_x);

		OP(69, adc, immediate);
		OP(65, adc, zero_page);
		OP(75, adc, zero_page_x);
		OP(6D, adc, absolute);
		OP(7D, adc, absolute_x);
		OP(79, adc, absolute_y);
		OP(61, adc, indexed_indirect);
		OP(71, adc, indirect_indexed);

		OP(29, and, immediate);
		OP(25, and, zero_page);
		OP(35, and, zero_page_x);
		OP(2D, and, absolute);
		OP(3D, and, absolute_x);
		OP(39, and, absolute_y);
		OP(21, and, indexed_indirect);
		OP(31, and, indirect_indexed);

		OP(0A, asl, accumulator);
		OP(06, asl, zero_page);
		OP(16, asl, zero_page_x);
		OP(0E, asl, absolute);
		OP(1E, asl, absolute_x);

		OP(90, bcc, relative);

		OP(B0, bcs, relative);

		OP(F0, beq, relative);

		OP(24, bit, zero_page);
		OP(2C, bit, absolute);

		OP(30, bmi, relative);

		OP(D0, bne, relative);

		OP(10, bpl, relative);


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
	cpu->sp = 0xFF;
	cpu->p = 0b00010000;
	cpu->a = 0x00;
	cpu->x = 0x00;
	cpu->y = 0x00;

	cpu->pc = ((word)(cpu->memory.data[0xFFFD] << 8)) | cpu->memory.data[0xFFFC];
}

bool cpu_get_c_flag(const cpu* cpu)
{
	return cpu->p & 0b00000001;
}

bool cpu_get_z_flag(const cpu* cpu)
{
	return cpu->p & 0b00000010;
}

bool cpu_get_i_flag(const cpu* cpu)
{
	return cpu->p & 0b00000100;
}

bool cpu_get_d_flag(const cpu* cpu)
{
	return cpu->p & 0b00001000;
}

bool cpu_get_b_flag(const cpu* cpu)
{
	return cpu->p & 0b00010000;
}

bool cpu_get_v_flag(const cpu* cpu)
{
	return cpu->p & 0b01000000;
}

bool cpu_get_n_flag(const cpu* cpu)
{
	return cpu->p & 0b10000000;
}


void cpu_set_c_flag(cpu* cpu, const unsigned char val)
{
	cpu->p ^= (-val ^ cpu->p) & (1UL << 0);
}

void cpu_set_z_flag(cpu* cpu, const unsigned char val)
{
	cpu->p ^= (-val ^ cpu->p) & (1UL << 1);
}

void cpu_set_i_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & (1UL << 2);
}

void cpu_set_d_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & (1UL << 3);
}

void cpu_set_b_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & (1UL << 4);
}

void cpu_set_v_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & (1UL << 6);
}

void cpu_set_n_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & (1UL << 7);
}




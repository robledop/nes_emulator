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

static void cpu_stack_push_16(cpu* cpu, const word val)
{
	cpu->memory.data[STACK_BASE + cpu->sp] = (val >> 8) & 0xFF;
	cpu->memory.data[STACK_BASE + (cpu->sp - 1)] = val & 0xFF;
	cpu->sp -= 2;
}

static void cpu_stack_push_8(cpu* cpu, const byte val)
{
	cpu->memory.data[STACK_BASE + cpu->sp--] = val;
}

static word cpu_stack_pop_16(cpu* cpu)
{
	const word result = cpu->memory.data[STACK_BASE + ((cpu->sp + 1) & 0xFF)] | cpu->memory.data[STACK_BASE + (((cpu->sp + 2) & 0xFF) << 8)];
	cpu->sp += 2;
	return result;
}

static byte cpu_stack_pop_8(cpu* cpu)
{
	return cpu->memory.data[STACK_BASE + ++cpu->sp];
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
static void AND(cpu* cpu, const address_mode address_mode)
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

// Force Interrupt
static void brk(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);

	cpu->pc++;
	cpu_stack_push_16(cpu, cpu->pc);
	cpu_stack_push_8(cpu, cpu->p | BREAK_FLAG);
	cpu_set_b_flag(cpu, 1);

	cpu->pc = cpu->memory.data[0xFFFE] | (word)(cpu->memory.data[0xFFFF] << 8);
}

// Branch if Overflow Clear
static void bvc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_v_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// Branch if Overflow Set
static void bvs(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_v_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += cpu->memory.data[address];
	}
}

// Clear Carry Flag
static void clc(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_c_flag(cpu, 0);
}

// Clear Decimal Mode
static void cld(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_d_flag(cpu, 0);
}

// Clear Interrupt Disable
static void cli(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_i_flag(cpu, 0);
}

// Clear Overflow Flag
static void clv(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_v_flag(cpu, 0);
}

// Compare
static void cmp(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = cpu->memory.data[address];

		cpu_set_c_flag(cpu, cpu->a >= memory ? 1 : 0);
		cpu_set_z_flag(cpu, cpu->a == memory ? 1 : 0);
		cpu_set_n_flag(cpu, cpu->a < memory ? 1 : 0);
}

// Compare X Register
static void cpx(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = cpu->memory.data[address];

	cpu_set_c_flag(cpu, cpu->x >= memory ? 1 : 0);
	cpu_set_z_flag(cpu, cpu->x == memory ? 1 : 0);
	cpu_set_n_flag(cpu, cpu->x < memory ? 1 : 0);
}

// Compare Y Register
static void cpy(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = cpu->memory.data[address];

	cpu_set_c_flag(cpu, cpu->y >= memory ? 1 : 0);
	cpu_set_z_flag(cpu, cpu->y == memory ? 1 : 0);
	cpu_set_n_flag(cpu, cpu->y < memory ? 1 : 0);
}

// Decrement Memory
static void dec(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->memory.data[address] -= 1;
	calc_negative(cpu, cpu->memory.data[address]);
	calc_zero(cpu, cpu->memory.data[address]);
}

// Decrement X Register
static void dex(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x -= 1;
	calc_negative(cpu, cpu->x);
	calc_zero(cpu, cpu->x);
}

// Decrement Y Register
static void dey(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->y -= 1;
	calc_negative(cpu, cpu->y);
	calc_zero(cpu, cpu->y);
}

// Exclusive OR
static void eor(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = cpu->memory.data[address];
	cpu->a ^= memory;
	calc_negative(cpu, cpu->a);
	calc_zero(cpu, cpu->a);
}

// Increment Memory
static void inc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->memory.data[address] += 1;
	calc_negative(cpu, cpu->memory.data[address]);
	calc_zero(cpu, cpu->memory.data[address]);
}

// Increment X Register
static void inx(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x += 1;
	calc_negative(cpu, cpu->x);
	calc_zero(cpu, cpu->x);
}

// Increment Y Register
static void iny(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->y += 1;
	calc_negative(cpu, cpu->y);
	calc_zero(cpu, cpu->y);
}

// Jump
static void jmp(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->pc = cpu->memory.data[address];

	// TODO: An original 6502 has does not correctly fetch the target address
	// if the indirect vector falls on a page boundary (e.g. $xxFF where xx is
	// any value from $00 to $FF). In this case fetches the LSB from $xxFF as
	// expected but takes the MSB from $xx00. This is fixed in some later chips
	// like the 65SC02 so for compatibility always ensure the indirect vector is
	// not at the end of the page.
}

// Jump to Subroutine
static void jsr(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == absolute);
	cpu->pc++;
	cpu_stack_push_16(cpu, cpu->pc - 1);
	const word address = get_memory_address(cpu, address_mode);
	cpu->pc = cpu->memory.data[address];
}

// Logical Shift Right
static void lsr(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0b00000001));
		cpu->a >>= 1;
		calc_negative(cpu, cpu->a);
		calc_zero(cpu, cpu->a);
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		const byte memory = cpu->memory.data[address];
		cpu_set_c_flag(cpu, (memory & 0b10000000));
		cpu->memory.data[address] >>= 1;
		calc_negative(cpu, cpu->memory.data[address]);
		calc_zero(cpu, cpu->memory.data[address]);
	}
}

// No Operation
static void nop(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
}

// Logical Inclusive OR
static void ora(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->a |= cpu->memory.data[address];
	calc_negative(cpu, cpu->a);
	calc_zero(cpu, cpu->a);
}

// Push Accumulator
static void pha(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_stack_push_8(cpu, cpu->a);
}

// Push Processor Status
static void php(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_stack_push_8(cpu, cpu->p);
}

// Pull Accumulator
static void pla(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->a = cpu_stack_pop_8(cpu);
}

// Pull Processor Status
static void plp(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->p = cpu_stack_pop_8(cpu);
}

// Rotate Left
static void rol(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0b10000000));
		cpu->a <<= 1;
		cpu->a |= cpu_get_c_flag(cpu) ? 1 : 0;
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		cpu_set_c_flag(cpu, (cpu->memory.data[address] & 0b10000000));
		cpu->memory.data[address] <<= 1;
		cpu->memory.data[address] |= cpu_get_c_flag(cpu) ? 1 : 0;
	}
}

//  Rotate Right
static void ror(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0b00000001));
		cpu->a >>= 1;
		cpu->a |= cpu_get_c_flag(cpu) ? 0b10000000 : 0;
		calc_negative(cpu, cpu->a);
		calc_zero(cpu, cpu->a);
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		const byte memory = cpu->memory.data[address];
		cpu_set_c_flag(cpu, (memory & 0b10000000));
		cpu->memory.data[address] >>= 1;
		cpu->memory.data[address] |= cpu_get_c_flag(cpu) ? 0b10000000 : 0;
		calc_negative(cpu, cpu->memory.data[address]);
		calc_zero(cpu, cpu->memory.data[address]);
	}
}

//  Return from Interrupt
static void rti(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->p = cpu_stack_pop_8(cpu);
	cpu->pc = cpu_stack_pop_8(cpu);
}

// Return from Subroutine
static void rts(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc = cpu_stack_pop_8(cpu) - 1;
}

// Subtract with Carry
static void sbc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = cpu->memory.data[address];
	const word result = ~(cpu->a + memory + (cpu_get_v_flag(cpu) ? 1 : 0));
	calc_carry(cpu, result);
	calc_overflow(cpu, result, memory);
	calc_zero(cpu, (byte)result);
	calc_negative(cpu, (byte)result);
	cpu->a = (byte)result;
}

// Set Carry Flag
static void sec(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu_set_c_flag(cpu, 1);
}

// Set Decimal Flag
static void sed(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu_set_d_flag(cpu, 1);
}

// Set Interrupt Disable
static void sei(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu_set_i_flag(cpu, 1);
}

// Store Accumulator
static void sta(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->memory.data[address] = cpu->a;
}

// Store X Register
static void stx(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->memory.data[address] = cpu->x;
}

// Store Y Register
static void sty(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->memory.data[address] = cpu->y;
}

// Transfer Accumulator to X
static void tax(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x = cpu->a;
	calc_zero(cpu, cpu->x);
	calc_negative(cpu, cpu->x);
}

// Transfer Accumulator to Y
static void tay(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->y = cpu->a;
	calc_zero(cpu, cpu->y);
	calc_negative(cpu, cpu->y);
}

// Transfer Stack Pointer to X
static void tsx(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x = cpu->sp;
	calc_zero(cpu, cpu->x);
	calc_negative(cpu, cpu->x);
}

// Transfer X to Accumulator
static void txa(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->a = cpu->x;
	calc_zero(cpu, cpu->a);
	calc_negative(cpu, cpu->a);
}

// Transfer X to Stack Pointer
static void txs(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->sp = cpu->x;
}

// Transfer Y to Accumulator
static void tya(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->a = cpu->y;
	calc_zero(cpu, cpu->a);
	calc_negative(cpu, cpu->a);
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

		OP(29, AND, immediate);
		OP(25, AND, zero_page);
		OP(35, AND, zero_page_x);
		OP(2D, AND, absolute);
		OP(3D, AND, absolute_x);
		OP(39, AND, absolute_y);
		OP(21, AND, indexed_indirect);
		OP(31, AND, indirect_indexed);

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

		OP(00, brk, implicit);

		OP(50, bvc, relative);

		OP(70, bvs, relative);

		OP(18, clc, implicit);

		OP(d8, cld, implicit);

		OP(58, cli, implicit);

		OP(B8, clv, implicit);

		OP(C9, cmp, immediate);
		OP(C5, cmp, zero_page);
		OP(D5, cmp, zero_page_x);
		OP(CD, cmp, absolute);
		OP(DD, cmp, absolute_x);
		OP(D9, cmp, absolute_y);
		OP(C1, cmp, indexed_indirect);
		OP(D1, cmp, indirect_indexed);

		OP(E0, cpx, immediate);
		OP(E4, cpx, zero_page);
		OP(EC, cpx, absolute);

		OP(C0, cpy, immediate);
		OP(C4, cpy, zero_page);
		OP(CC, cpy, absolute);

		OP(C6, dec, zero_page);
		OP(D6, dec, zero_page_x);
		OP(CE, dec, absolute);
		OP(DE, dec, absolute_x);

		OP(CA, dex, implicit);

		OP(88, dey, implicit);

		OP(49, eor, immediate);
		OP(45, eor, zero_page);
		OP(55, eor, zero_page_x);
		OP(4D, eor, absolute);
		OP(5D, eor, absolute_x);
		OP(59, eor, absolute_y);
		OP(41, eor, indexed_indirect);
		OP(51, eor, indirect_indexed);

		OP(E6, inc, zero_page);
		OP(F6, inc, zero_page_x);
		OP(EE, inc, absolute);
		OP(FE, inc, absolute_x);

		OP(E8, inx, implicit);

		OP(C8, iny, implicit);

		OP(4C, jmp, absolute);
		OP(6C, jmp, indirect);

		OP(20, jsr, absolute);

		OP(4A, lsr, accumulator);
		OP(46, lsr, zero_page);
		OP(56, lsr, zero_page_x);
		OP(4E, lsr, absolute);
		OP(5E, lsr, absolute_x);

		OP(EA, nop, implicit);

		OP(09, ora, immediate);
		OP(05, ora, zero_page);
		OP(15, ora, zero_page_x);
		OP(0D, ora, absolute);
		OP(1D, ora, absolute_x);
		OP(19, ora, absolute_y);
		OP(01, ora, indexed_indirect);
		OP(11, ora, indirect_indexed);

		OP(48, pha, implicit);

		OP(08, php, implicit);

		OP(68, pha, implicit);

		OP(28, plp, implicit);

		OP(2A, rol, accumulator);
		OP(26, rol, zero_page);
		OP(36, rol, zero_page_x);
		OP(2E, rol, absolute);
		OP(3E, rol, absolute_x);

		OP(6A, ror, accumulator);
		OP(66, ror, zero_page);
		OP(76, ror, zero_page_x);
		OP(6E, ror, absolute);
		OP(7E, ror, absolute_x);

		OP(40, rti, implicit);

		OP(60, rts, implicit);

		OP(E9, sbc, immediate);
		OP(E5, sbc, zero_page);
		OP(F5, sbc, zero_page_x);
		OP(ED, sbc, absolute);
		OP(FD, sbc, absolute_x);
		OP(F9, sbc, absolute_y);
		OP(E1, sbc, indexed_indirect);
		OP(F1, sbc, indirect_indexed);

		OP(38, sec, implicit);

		OP(F8, sed, implicit);

		OP(78, sei, implicit);

		OP(85, sta, zero_page);
		OP(95, sta, zero_page_x);
		OP(8D, sta, absolute);
		OP(9D, sta, absolute_x);
		OP(99, sta, absolute_y);
		OP(81, sta, indexed_indirect);
		OP(91, sta, indirect_indexed);

		OP(86, stx, zero_page);
		OP(96, stx, zero_page_y);
		OP(8E, stx, absolute);

		OP(84, sty, zero_page);
		OP(94, sty, zero_page_x);
		OP(8C, sty, absolute);

		OP(AA, tax, implicit);

		OP(A8, tay, implicit);

		OP(BA, tsx, implicit);

		OP(8A, txa, implicit);

		OP(9A, tsx, implicit);

		OP(98, tya, implicit);

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




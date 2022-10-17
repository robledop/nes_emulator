#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include "memory.h"


static byte read_memory(cpu* cpu, const word address);
static void write_memory(cpu* cpu, const word address, const byte value);

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

static void calc_add(cpu* cpu, const byte argument)
{
	const word result = cpu->a + argument + (cpu_get_c_flag(cpu) ? 1 : 0);
	calc_carry(cpu, result);
	calc_overflow(cpu, result, argument);
	cpu->a = result & 0xFF;
	calc_zero(cpu, cpu->a);
	calc_negative(cpu, cpu->a);
}

byte cpu_read_memory(const cpu* cpu, const word address)
{
	return cpu->memory.data[address];
}

void cpu_write_memory(cpu* cpu, const word address, const byte value)
{
	cpu->memory.data[address] = value;
}

static word get_memory_address(cpu* cpu, const address_mode address_mode)
{
	switch (address_mode) {  // NOLINT(hicpp-multiway-paths-covered)
		case implicit:
			assert(false);
			break;

		case accumulator:
			break;

		case immediate:
			return cpu->pc++;

		case zero_page:
		{
			const byte value = read_memory(cpu, cpu->pc);
			cpu->pc++;
			return value;
		}

		case zero_page_x:
		{
			const byte value = read_memory(cpu, cpu->pc);
			cpu->pc++;
			return value + cpu->x;
		}

		case zero_page_y:
		{
			const byte value = read_memory(cpu, cpu->pc);
			cpu->pc++;
			return value + cpu->y;
		}

		case relative:
		{
			const byte value = read_memory(cpu, cpu->pc);
			cpu->pc++;
			return value;
		}

		case absolute:
		{
			const byte low_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			const byte high_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			return  ((word)(high_byte << 8)) | low_byte;
		}

		case absolute_x:
		{
			const byte low_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			const byte high_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->x;
			return address;
		}

		case absolute_y:
		{
			const byte low_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			const byte high_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->y;
			return address;
		}

		case indirect:
		{
			const byte low_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			const byte high_byte = read_memory(cpu, cpu->pc);
			cpu->pc++;
			const word address = ((word)(high_byte << 8)) | low_byte;

			const byte indirect_lo_byte = read_memory(cpu, address);
			const byte indirect_hi_byte = read_memory(cpu, address + 1);

			const word indirect_address = ((word)(indirect_hi_byte << 8)) | indirect_lo_byte;

			return indirect_address;
		}

		case indexed_indirect:
		{
			const byte vector = read_memory(cpu, cpu->pc) + cpu->x;
			cpu->pc++;
			const byte low_byte = read_memory(cpu, vector);
			const byte high_byte = read_memory(cpu, vector + 1);
			return  ((word)(high_byte << 8)) | low_byte;
		}

		case indirect_indexed:
		{
			const byte vector = read_memory(cpu, cpu->pc);
			cpu->pc++;
			const byte low_byte = read_memory(cpu, vector);
			const byte high_byte = read_memory(cpu, vector + 1);

			word address = ((word)(high_byte << 8)) | low_byte;
			address += cpu->y;
			return  address;
		}
	}

	return 0;
}

void cpu_stack_push_16(cpu* cpu, const word val)
{
	write_memory(cpu, STACK_BASE + cpu->sp, (val >> 8) & 0xFF);
	write_memory(cpu, STACK_BASE + (cpu->sp - 1), val & 0xFF);
	cpu->sp -= 2;
}

void cpu_stack_push_8(cpu* cpu, const byte val)
{
	write_memory(cpu, STACK_BASE + cpu->sp, val);
	cpu->sp--;
}

word cpu_stack_pop_16(cpu* cpu)
{
	const word result = (word)(read_memory(cpu, STACK_BASE + (cpu->sp + 2)) << 8) | (word)read_memory(cpu, STACK_BASE + (cpu->sp + 1));
	cpu->sp += 2;
	return result;
}

byte cpu_stack_pop_8(cpu* cpu)
{
	cpu->sp++;
	return  read_memory(cpu, STACK_BASE + cpu->sp);
}

static byte read_memory(cpu* cpu, word address)
{
	// handle memory mirroring
	while (address >= 0x2008 && address < 0x4000)
	{
		address -= 8;
	}

	while (address >= 0x0800 && address < 0x2000)
	{
		address -= 0x0800;
	}

	switch (address)
	{
		case PPU_CTRL:
		case PPU_MASK:
		case PPU_SCROLL:
			// Do nothing. Write-only registers
			return 0;

		case PPU_STATUS:
			cpu->ppu.ppu_latch = false;
			return cpu->ppu.registers.ppu_status;
		case OAM_ADDR:
			return cpu->ppu.registers.oam_addr;
		case OAM_DATA:
			return cpu->ppu.registers.oam_data;
		case PPU_ADDR:
			return cpu->ppu.registers.ppu_addr;
		case PPU_DATA:
			return cpu->ppu.registers.ppu_data;
		case CONTROLLER_1:
			return read_next_button(cpu->controller);
		default:
			return cpu_read_memory(cpu, address);
	}
}

static void write_memory(cpu* cpu, word address, const byte value)
{
	// handle memory mirroring
	while (address >= 0x2008 && address < 0x4000)
	{
		address -= 8;
	}
	while (address >= 0x0800 && address < 0x2000)
	{
		address -= 0x0800;
	}

	switch (address)
	{
		case PPU_CTRL:
			cpu->ppu.registers.ppu_ctrl = value;
			break;
		case PPU_MASK:
			cpu->ppu.registers.ppu_mask = value;
			break;
		case PPU_STATUS:
			// Do nothing. PPU_STATUS is read-only
			break;
		case OAM_ADDR:
			cpu->ppu.registers.oam_addr = value;
			break;
		case OAM_DATA:
			cpu->ppu.registers.oam_data = value;
			break;

		case PPU_SCROLL:
		{
			if (cpu->ppu.ppu_latch)
			{
				cpu->ppu.registers.ppu_scroll_y = value;
				cpu->ppu.ppu_latch = false;
			}
			else
			{
				cpu->ppu.registers.ppu_scroll_x = value;
				cpu->ppu.ppu_latch = true;
			}
		}
		break;

		case PPU_ADDR:
		{
			if (cpu->ppu.ppu_latch)
			{
				cpu->ppu.ppu_data_addr |= (word)value;
				cpu->ppu.ppu_latch = false;
			}
			else
			{
				cpu->ppu.ppu_data_addr = (word)(value << 8);
				cpu->ppu.ppu_latch = true;
			}
			assert(cpu->ppu.ppu_data_addr < VRAM_SIZE);
		}
		break;

		case PPU_DATA:
			assert(cpu->ppu.ppu_data_addr <= VRAM_SIZE - 1);
			cpu->ppu.memory.data[cpu->ppu.ppu_data_addr] = value;
			cpu->ppu.ppu_data_addr++;
			break;

		case OAM_DMA:
		{
			for (word i = 0; i < 256; ++i)
			{
				const word oam_copy_address = (word)(value << 8) + i;
				cpu->ppu.oam.data[i] = read_memory(cpu, oam_copy_address);
			}
		}
		break;

		case CONTROLLER_1:
			write_controller(cpu->controller, value);
			break;
		default:
			cpu_write_memory(cpu, address, value);
	}
}

static void lda(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->a = read_memory(cpu, address);

	calc_zero(cpu, cpu->a);
	calc_negative(cpu, cpu->a);

#ifdef LOGGING
	printf("LDA %x\n", cpu->a);
#endif
}

static void ldx(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->x = read_memory(cpu, address);
	calc_zero(cpu, cpu->x);
	calc_negative(cpu, cpu->x);
#ifdef LOGGING
	printf("LDX %x\n", cpu->x);
#endif
}

static void ldy(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->y = read_memory(cpu, address);
	calc_zero(cpu, cpu->y);
	calc_negative(cpu, cpu->y);
#ifdef LOGGING
	printf("LDY %x\n", cpu->y);
#endif
}

// Add with Carry
static void adc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);
	calc_add(cpu, memory);

#ifdef LOGGING
	printf("ADC %x\n", memory);
#endif
}

// Logical AND
static void AND(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte value = read_memory(cpu, address);
	cpu->a &= value;
	calc_negative(cpu, cpu->a);
	calc_zero(cpu, cpu->a);
#ifdef LOGGING
	printf("AND %x\n", read_memory(cpu, address));
#endif
}

// Arithmetic Shift Left
static void asl(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0x10000000 ? 1 : 0));
		cpu->a <<= 1;

		calc_negative(cpu, cpu->a);
		calc_zero(cpu, cpu->a);
#ifdef LOGGING
		puts("ASL");
#endif
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		const byte value = read_memory(cpu, address);
		cpu_set_c_flag(cpu, (value & 0x10000000 ? 1 : 0));
		const byte new_value = (byte)(value << 1);
		write_memory(cpu, address, new_value);

		calc_negative(cpu, new_value);
		calc_zero(cpu, new_value);

#ifdef LOGGING
		printf("ASL %x <%x>\n", address, new_value);
#endif
	}
}

// Branch if Carry Clear
static void bcc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_c_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
	}
	else
	{
		cpu->pc++;
	}
#ifdef LOGGING
	printf("BCC %x\n", cpu->pc);
#endif
}

// Branch if Carry Set
static void bcs(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_c_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
	}
	else
	{
		cpu->pc++;
	}

#ifdef LOGGING
	printf("BCS %x\n", cpu->pc);
#endif
}

// Branch if Equal
static void beq(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_z_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
	}
	else
	{
		cpu->pc++;
	}

#ifdef LOGGING
	printf("BEQ %x\n", cpu->pc);
#endif
}

// Bit Test
static void bit(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);
	const byte result = cpu->a & memory;

	cpu->p = memory & 0b11000000;
	cpu_set_z_flag(cpu, result);

#ifdef LOGGING
	printf("BIT %x\n", address);
#endif
}

// Branch if Minus
static void bmi(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_n_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
	}
	else
	{
		cpu->pc++;
	}
#ifdef LOGGING
	printf("BMI %x\n", cpu->pc);
#endif
}

// Branch if Not Equal
static void bne(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_z_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
	}
	else
	{
		cpu->pc++;
	}
#ifdef LOGGING
	printf("BNE %x\n", cpu->pc);
#endif
}

// Branch if Positive
static void bpl(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_n_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
#ifdef LOGGING
		printf("BPL %x\n", address);
#endif
	}
	else
	{
		cpu->pc++;
#ifdef LOGGING
		printf("BPL %x\n", cpu->pc);
#endif
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

	cpu->pc = read_memory(cpu, 0xFFFE) | (word)(read_memory(cpu, 0xFFFF) << 8);

#ifdef LOGGING
	printf("BRK\n");
#endif
}

// Branch if Overflow Clear
static void bvc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (!cpu_get_v_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
	}
#ifdef LOGGING
	printf("BVC %x\n", cpu->pc);
#endif
}

// Branch if Overflow Set
static void bvs(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (cpu_get_v_flag(cpu))
	{
		const word address = get_memory_address(cpu, address_mode);
		cpu->pc += (char)address;
	}

#ifdef LOGGING
	printf("BVS %x\n", cpu->pc);
#endif
}

// Clear Carry Flag
static void clc(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_c_flag(cpu, 0);
#ifdef LOGGING
	puts("CLC");
#endif
}

// Clear Decimal Mode
static void cld(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_d_flag(cpu, 0);

#ifdef LOGGING
	puts("CLD");
#endif
}

// Clear Interrupt Disable
static void cli(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_i_flag(cpu, 0);

#ifdef LOGGING
	puts("CLI");
#endif
}

// Clear Overflow Flag
static void clv(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_v_flag(cpu, 0);

#ifdef LOGGING
	puts("CLV");
#endif
}

// Compare
static void cmp(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);

	cpu_set_c_flag(cpu, cpu->a >= memory ? 1 : 0);
	cpu_set_z_flag(cpu, cpu->a == memory ? 1 : 0);
	cpu_set_n_flag(cpu, cpu->a < memory ? 1 : 0);

#ifdef LOGGING
	printf("CMP %x\n", memory);
#endif
}

// Compare X Register
static void cpx(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);

	cpu_set_c_flag(cpu, cpu->x >= memory ? 1 : 0);
	cpu_set_z_flag(cpu, cpu->x == memory ? 1 : 0);
	cpu_set_n_flag(cpu, cpu->x < memory ? 1 : 0);

#ifdef LOGGING
	printf("CPX %x\n", memory);
#endif
}

// Compare Y Register
static void cpy(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);

	cpu_set_c_flag(cpu, cpu->y >= memory ? 1 : 0);
	cpu_set_z_flag(cpu, cpu->y == memory ? 1 : 0);
	cpu_set_n_flag(cpu, cpu->y < memory ? 1 : 0);

#ifdef LOGGING
	printf("CPY %x\n", memory);
#endif
}

// Decrement Memory
static void dec(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);
	const byte new_value = memory - 1;
	write_memory(cpu, address, new_value);
	calc_negative(cpu, new_value);
	calc_zero(cpu, new_value);

#ifdef LOGGING
	printf("DEC %x\n", address);
#endif
}

// Decrement X Register
static void dex(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x -= 1;
	calc_negative(cpu, cpu->x);
	calc_zero(cpu, cpu->x);

#ifdef LOGGING
	puts("DEX");
#endif
}

// Decrement Y Register
static void dey(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->y -= 1;
	calc_negative(cpu, cpu->y);
	calc_zero(cpu, cpu->y);

#ifdef LOGGING
	puts("DEY");
#endif
}

// Exclusive OR
static void eor(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);
	cpu->a ^= memory;
	calc_negative(cpu, cpu->a);
	calc_zero(cpu, cpu->a);

#ifdef LOGGING
	printf("EOR %x\n", memory);
#endif
}

// Increment Memory
static void inc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);
	const byte new_value = memory + 1;
	write_memory(cpu, address, new_value);
	calc_negative(cpu, new_value);
	calc_zero(cpu, new_value);

#ifdef LOGGING
	printf("INC %x\n", address);
#endif
}

// Increment X Register
static void inx(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x += 1;
	calc_negative(cpu, cpu->x);
	calc_zero(cpu, cpu->x);

#ifdef LOGGING
	puts("INX");
#endif
}

// Increment Y Register
static void iny(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->y += 1;
	calc_negative(cpu, cpu->y);
	calc_zero(cpu, cpu->y);

#ifdef LOGGING
	puts("INY");
#endif
}

// Jump
static void jmp(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);

	// An original 6502 has a bug where it does not correctly fetch the target address
	// if the indirect vector falls on a page boundary (e.g. $xxFF where xx is
	// any value from $00 to $FF). In this case fetches the LSB from $xxFF as
	// expected but takes the MSB from $xx00. This is fixed in some later chips
	// like the 65SC02 so for compatibility always ensure the indirect vector is
	// not at the end of the page.
	assert(!(address_mode == indirect && (address & 0x00FF) == 0xFF));

	cpu->pc = address;

#ifdef LOGGING
	puts("JMP");
#endif
}

// Jump to Subroutine
static void jsr(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == absolute);
	cpu->pc++;
	cpu_stack_push_16(cpu, cpu->pc + 2);
	const word address = get_memory_address(cpu, address_mode);
	cpu->pc = address;

#ifdef LOGGING
	printf("JSR %x\n", address);
#endif
}

// Logical Shift Right
static void lsr(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0b00000001) ? 1 : 0);
		cpu->a >>= 1;
		calc_negative(cpu, cpu->a);
		calc_zero(cpu, cpu->a);

#ifdef LOGGING
		printf("LSR\n");
#endif
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		byte memory = read_memory(cpu, address);
		cpu_set_c_flag(cpu, (memory & 0b00000001) ? 1 : 0);
		write_memory(cpu, address, memory >> 1);
		memory = read_memory(cpu, address);
		calc_negative(cpu, memory);
		calc_zero(cpu, memory);

#ifdef LOGGING
		printf("LSR %x\n", address);
#endif
	}
}

// No Operation
static void nop(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;

#ifdef LOGGING
	puts("NOP");
#endif
}

// Logical Inclusive OR
static void ora(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	cpu->a |= read_memory(cpu, address);
	calc_negative(cpu, cpu->a);
	calc_zero(cpu, cpu->a);

#ifdef LOGGING
	printf("ORA %x\n", address);
#endif
}

// Push Accumulator
static void pha(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_stack_push_8(cpu, cpu->a);

#ifdef LOGGING
	puts("PHA");
#endif
}

// Push Processor Status
static void php(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_stack_push_8(cpu, cpu->p | UNUSED_FLAG | B_FLAG);

#ifdef LOGGING
	puts("PHP");
#endif
}

// Pull Accumulator
static void pla(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->a = cpu_stack_pop_8(cpu);
	calc_negative(cpu, cpu->a);
	calc_zero(cpu, cpu->a);

#ifdef LOGGING
	puts("PLA");
#endif
}

// Pull Processor Status
static void plp(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->p = cpu_stack_pop_8(cpu);

#ifdef LOGGING
	puts("PLP");
#endif
}

// Rotate Left
static void rol(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const bool current_carry_flag = cpu_get_c_flag(cpu);
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0b10000000) ? 1 : 0);
		cpu->a <<= 1;
		cpu->a |= current_carry_flag ? 1 : 0;
		calc_zero(cpu, cpu->a);
		calc_negative(cpu, cpu->a);

#ifdef LOGGING
		puts("ROL");
#endif
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		const byte memory = read_memory(cpu, address);
		cpu_set_c_flag(cpu, (memory & 0b10000000) ? 1 : 0);
		byte new_value = (byte)(memory << 1);
		new_value |= current_carry_flag ? 1 : 0;
		write_memory(cpu, address, new_value);
		calc_zero(cpu, new_value);
		calc_negative(cpu, new_value);

#ifdef LOGGING
		printf("ROL %x\n", address);
#endif
	}
}

//  Rotate Right
static void ror(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const bool current_carry_flag = cpu_get_c_flag(cpu);
	if (address_mode == accumulator)
	{
		cpu_set_c_flag(cpu, (cpu->a & 0b00000001) ? 1 : 0);
		cpu->a >>= 1;
		cpu->a |= current_carry_flag ? 0b10000000 : 0;
		calc_negative(cpu, cpu->a);
		calc_zero(cpu, cpu->a);

#ifdef LOGGING
		puts("ROR");
#endif
	}
	else {
		const word address = get_memory_address(cpu, address_mode);
		const byte memory = read_memory(cpu, address);
		cpu_set_c_flag(cpu, (memory & 0b00000001) ? 1 : 0);
		byte new_value = memory >> 1;
		new_value |= current_carry_flag ? 0b10000000 : 0;
		write_memory(cpu, address, new_value);
		calc_negative(cpu, new_value);
		calc_zero(cpu, new_value);

#ifdef LOGGING
		printf("ROR %x\n", address);
#endif
	}
}

//  Return from Interrupt
static void rti(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->p = cpu_stack_pop_8(cpu);
	cpu->pc = cpu_stack_pop_16(cpu);

#ifdef LOGGING
	puts("RTI");
#endif
}

// Return from Subroutine
static void rts(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	const word value = cpu_stack_pop_16(cpu);
	cpu->pc = value;

#ifdef LOGGING
	puts("RTS");
#endif
}

// Subtract with Carry
static void sbc(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	const byte memory = read_memory(cpu, address);
	calc_add(cpu, ~memory);

#ifdef LOGGING
	printf("SBC %x\n", address);
#endif
}

// Set Carry Flag
static void sec(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_c_flag(cpu, 1);

#ifdef LOGGING
	puts("SEC");
#endif
}

// Set Decimal Flag
static void sed(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_d_flag(cpu, 1);

#ifdef LOGGING
	puts("SED");
#endif
}

// Set Interrupt Disable
static void sei(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu_set_i_flag(cpu, 1);

#ifdef LOGGING
	puts("SEI");
#endif
}

// Store Accumulator
static void sta(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	printf("STA %x (%x)\n", address, cpu->a);
	write_memory(cpu, address, cpu->a);

#ifdef LOGGING
#endif
}

// Store X Register
static void stx(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	write_memory(cpu, address, cpu->x);

#ifdef LOGGING
	printf("STX %x\n", address);
#endif
}

// Store Y Register
static void sty(cpu* cpu, const address_mode address_mode)
{
	cpu->pc++;
	const word address = get_memory_address(cpu, address_mode);
	write_memory(cpu, address, cpu->y);

#ifdef LOGGING
	printf("STY %x\n", address);
#endif
}

// Transfer Accumulator to X
static void tax(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x = cpu->a;
	calc_zero(cpu, cpu->x);
	calc_negative(cpu, cpu->x);

#ifdef LOGGING
	puts("TAX");
#endif
}

// Transfer Accumulator to Y
static void tay(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->y = cpu->a;
	calc_zero(cpu, cpu->y);
	calc_negative(cpu, cpu->y);

#ifdef LOGGING
	puts("TAY");
#endif
}

// Transfer Stack Pointer to X
static void tsx(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->x = cpu->sp;
	calc_zero(cpu, cpu->x);
	calc_negative(cpu, cpu->x);

#ifdef LOGGING

	puts("TSX");
#endif
}

// Transfer X to Accumulator
static void txa(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->a = cpu->x;
	calc_zero(cpu, cpu->a);
	calc_negative(cpu, cpu->a);

#ifdef LOGGING

	puts("TXA");
#endif
}

// Transfer X to Stack Pointer
static void txs(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->sp = cpu->x;

#ifdef LOGGING

	puts("TXS");
#endif
}

// Transfer Y to Accumulator
static void tya(cpu* cpu, const address_mode address_mode)
{
	assert(address_mode == implicit);
	cpu->pc++;
	cpu->a = cpu->y;
	calc_zero(cpu, cpu->a);
	calc_negative(cpu, cpu->a);

#ifdef LOGGING
	puts("TYA");
#endif

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
		OP(1A, nop, implicit);
		OP(3A, nop, implicit);

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

		OP(68, pla, implicit);

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

		OP(9A, txs, implicit);

		OP(98, tya, implicit);

		default:
			cpu->pc++;
			printf("Unsupported opcode:%x\nPC:%x\n", instruction, cpu->pc);
	}
}

void cpu_clear_memory(cpu* cpu)
{
	memset(&cpu->memory, 0, MAX_MEMORY);
}

void ppu_clear_memory(ppu* ppu)
{
	memset(&ppu->memory.data, 0, VRAM_SIZE);
	memset(&ppu->oam.data, 0, OAM_SIZE);

	ppu->ppu_data_addr = 0x00;
	ppu->ppu_latch = false;
	ppu->ppu_latch = false;
}

void ppu_clear_registers(ppu* ppu)
{
	ppu->registers.oam_addr = 0x00;
	ppu->registers.oam_data = 0x00;
	ppu->registers.oam_dma = 0x00;
	ppu->registers.ppu_addr = 0x00;
	ppu->registers.ppu_ctrl = 0x00;
	ppu->registers.ppu_data = 0x00;
	ppu->registers.ppu_mask = 0x00;
	ppu->registers.ppu_scroll_x = 0x00;
	ppu->registers.ppu_scroll_y = 0x00;
	ppu->registers.ppu_status = 0x00;
}

void cpu_init(cpu* cpu, const word prg_size)
{
	cpu->sp = 0xFF;
	cpu->p = 0b00100000;
	cpu->a = 0x00;
	cpu->x = 0x00;
	cpu->y = 0x00;

	cpu->pc = ((word)(read_memory(cpu, 0x8000 + prg_size - 3) << 8)) | read_memory(cpu, 0x8000 + prg_size - 4);

	cpu->nmi_prt = ((word)(read_memory(cpu, 0x8000 + prg_size - 5) << 8)) | read_memory(cpu, 0x8000 + prg_size - 6);

	ppu_clear_memory(&cpu->ppu);
	ppu_clear_registers(&cpu->ppu);
	init_controller(cpu->controller);
}



bool cpu_get_c_flag(const cpu* cpu)
{
	return cpu->p & C_FLAG;
}

bool cpu_get_z_flag(const cpu* cpu)
{
	return cpu->p & Z_FLAG;
}

bool cpu_get_i_flag(const cpu* cpu)
{
	return cpu->p & I_FLAG;
}

bool cpu_get_d_flag(const cpu* cpu)
{
	return cpu->p & D_FLAG;
}

bool cpu_get_b_flag(const cpu* cpu)
{
	return cpu->p & B_FLAG;
}

bool cpu_get_v_flag(const cpu* cpu)
{
	return cpu->p & V_FLAG;
}

bool cpu_get_n_flag(const cpu* cpu)
{
	return cpu->p & N_FLAG;
}


void cpu_set_c_flag(cpu* cpu, const unsigned char val)
{
	cpu->p ^= (-val ^ cpu->p) & C_FLAG;
}

void cpu_set_z_flag(cpu* cpu, const unsigned char val)
{
	cpu->p ^= (-val ^ cpu->p) & Z_FLAG;
}

void cpu_set_i_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & I_FLAG;
}

void cpu_set_d_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & D_FLAG;
}

void cpu_set_b_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & B_FLAG;
}

void cpu_set_v_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & V_FLAG;
}

void cpu_set_n_flag(cpu* cpu, const char val)
{
	cpu->p ^= (-val ^ cpu->p) & N_FLAG;
}

void cpu_call_nmi(cpu* cpu)
{
	cpu_stack_push_16(cpu, cpu->pc);
	cpu_stack_push_8(cpu, cpu->p);
	cpu->pc = cpu->nmi_prt;
}
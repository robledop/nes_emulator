#include "pch.h"

#include "CppUnitTest.h"
extern "C" {
#include "../nes_emulator/cpu.h"
#include "../nes_emulator/nes.h"
}

#pragma warning( push )
#pragma warning( disable : 6262)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace nes_emulator_tests
{
	TEST_CLASS(ror_instruction_tests)
	{
	public:
		TEST_METHOD(ROR_accumulator)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x6A;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.a = 0b00000001;
			cpu_exec(&nes.cpu, 0x6A);

			Assert::IsTrue(nes.cpu.pc == 0x8001);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));


			nes.cpu.a = 0b00000001;

			// and place the bit from the carry on the accumulator
			cpu_set_c_flag(&nes.cpu, 1);

			cpu_exec(&nes.cpu, 0x6A);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			// it should move bit 3 to the right
			nes.cpu.a = 0b00001000;

			cpu_exec(&nes.cpu, 0x6A);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0b10000100);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(ROR_absolute)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x6E;

			// absolute address 0x0000
			nes.cpu.memory.data[0x8001] = 0x00;
			nes.cpu.memory.data[0x8002] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.memory.data[0x0000] = 0b00000001;

			cpu_exec(&nes.cpu, 0x6E);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.memory.data[0x0000] == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0000] = 0b00000001;

			cpu_exec(&nes.cpu, 0x6E);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.memory.data[0x0000] == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0000] = 0b00001000;

			cpu_exec(&nes.cpu, 0x6E);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.memory.data[0x0000] == 0b10000100);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(ROR_absolute_x)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x7E;

			// absolute address 0x0000
			nes.cpu.memory.data[0x8001] = 0x00;
			nes.cpu.memory.data[0x8002] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x01;
			nes.cpu.memory.data[0x0001] = 0b00000001;

			cpu_exec(&nes.cpu, 0x7E);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.memory.data[0x0001] == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0001] = 0b00000001;

			cpu_exec(&nes.cpu, 0x7E);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.memory.data[0x0001] == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0001] = 0b00001000;

			cpu_exec(&nes.cpu, 0x7E);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.memory.data[0x0001] == 0b10000100);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(ROR_zero_page)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x66;

			// zero page address 0x10
			nes.cpu.memory.data[0x8001] = 0x10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.memory.data[0x0010] = 0b00000001;

			cpu_exec(&nes.cpu, 0x66);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.memory.data[0x0010] == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0010] = 0b00000001;

			cpu_exec(&nes.cpu, 0x66);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.memory.data[0x0010] == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0010] = 0b00001000;

			cpu_exec(&nes.cpu, 0x66);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.memory.data[0x0010] == 0b10000100);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(ROL_zero_page_x)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x76;

			// zero page address 0x10
			nes.cpu.memory.data[0x8001] = 0x10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x01;
			nes.cpu.memory.data[0x0011] = 0b00000001;

			cpu_exec(&nes.cpu, 0x76);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.memory.data[0x0011] == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0011] = 0b00000001;

			cpu_exec(&nes.cpu, 0x76);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.memory.data[0x0011] == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));

			nes.cpu.pc = 0x8000;
			nes.cpu.memory.data[0x0011] = 0b00001000;

			cpu_exec(&nes.cpu, 0x76);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.memory.data[0x0011] == 0b10000100);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}
	};
}

#pragma warning( pop ) 

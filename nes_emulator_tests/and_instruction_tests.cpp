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
	TEST_CLASS(and_instruction_tests)
	{
	public:
		TEST_METHOD(AND_immediate)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x29;

			// immediate
			nes.cpu.memory.data[0x8001] = 0xF0;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x29);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(AND_zero_page)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x25;

			// zero page address 0x0001
			nes.cpu.memory.data[0x8001] = 0x01;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.memory.data[0x0001] = 0xF0;
			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x25);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(AND_zero_page_x)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x35;

			// zero page address 0x0001
			nes.cpu.memory.data[0x8001] = 0x01;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x01;
			nes.cpu.memory.data[0x0002] = 0xF0;
			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x35);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(AND_absolute)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x35;

			// absolute address 0x0001
			nes.cpu.memory.data[0x8001] = 0x01;
			nes.cpu.memory.data[0x8002] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.memory.data[0x0001] = 0xF0;
			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x35);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(AND_absolute_x)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x3D;

			// absolute address 0x0001
			nes.cpu.memory.data[0x8001] = 0x01;
			nes.cpu.memory.data[0x8002] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x01;
			nes.cpu.memory.data[0x0002] = 0xF0;
			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x3D);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(AND_absolute_y)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x39;

			// absolute address 0x0001
			nes.cpu.memory.data[0x8001] = 0x01;
			nes.cpu.memory.data[0x8002] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x01;
			nes.cpu.memory.data[0x0002] = 0xF0;
			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x39);

			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(AND_indexed_indirect)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x21;

			// absolute address 0x0001
			nes.cpu.memory.data[0x8001] = 0x01;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x01;
			nes.cpu.memory.data[0x0001] = 0x01;
			nes.cpu.memory.data[0x0002] = 0x04;
			nes.cpu.memory.data[0x0003] = 0x00;

			nes.cpu.memory.data[0x0004] = 0xF0;
			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x21);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_b_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_d_flag(&nes.cpu));
		}

		TEST_METHOD(AND_indirect_indexed)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x31;

			// absolute address 0x0001
			nes.cpu.memory.data[0x8001] = 0x01;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x01;
			nes.cpu.memory.data[0x0001] = 0x03;
			nes.cpu.memory.data[0x0002] = 0x00;

			nes.cpu.memory.data[0x0004] = 0xF0;
			nes.cpu.a = 0b10001111;

			cpu_exec(&nes.cpu, 0x31);

			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
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

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
	TEST_CLASS(sbc_instruction_tests)
	{
	public:

		TEST_METHOD(SBC_immediate_positive_number_no_overflow_no_carry)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xE9;

			// immediate value
			nes.cpu.memory.data[0x8001] = 0x20;

			cpu_init(&nes.cpu, 0x8000);
			nes.cpu.a = 0x30;
			cpu_exec(&nes.cpu, 0xE9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x0F);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
		}

		TEST_METHOD(SBC_immediate_positive_number_carry)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xE9;

			// immediate value
			nes.cpu.memory.data[0x8001] = 0x20;

			cpu_init(&nes.cpu, 0x8000);
			cpu_set_c_flag(&nes.cpu, 1);
			nes.cpu.a = 0x30;
			cpu_exec(&nes.cpu, 0xE9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x10);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
		}

		TEST_METHOD(ADC_immediate_positive_number_overflow)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xE9;

			// immediate value
			nes.cpu.memory.data[0x8001] = 0b01111111;

			cpu_init(&nes.cpu, 0x8000);
			nes.cpu.a = 0b00000001;
			cpu_exec(&nes.cpu, 0xE9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_v_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(ADC_immediate_negative_number_carry_overflow)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xE9;

			// immediate value
			nes.cpu.memory.data[0x8001] = -127;

			cpu_init(&nes.cpu, 0x8000);
			nes.cpu.a = -127;
			cpu_exec(&nes.cpu, 0xE9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x02);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_v_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
		}
	};
}

#pragma warning( pop ) 

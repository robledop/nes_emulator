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
	TEST_CLASS(adc_instruction_tests)
	{
	public:

		TEST_METHOD(ADC_immediate_positive_number_no_overflow_no_carry)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0x69;

			// immediate value
			cpu.memory.data[0x8001] = 0x11;

			cpu_init(&cpu, 0x8000);
			cpu.a = 0x22;
			cpu_exec(&cpu, 0x69);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.a == 0x33);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
			Assert::IsFalse(cpu_get_v_flag(&cpu));
			Assert::IsFalse(cpu_get_c_flag(&cpu));
		}

		TEST_METHOD(ADC_immediate_positive_number_carry)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0x69;

			// immediate value
			cpu.memory.data[0x8001] = 0xFF;

			cpu_init(&cpu, 0x8000);
			cpu.a = 0x22;
			cpu_exec(&cpu, 0x69);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.a == 0x21);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
			Assert::IsFalse(cpu_get_v_flag(&cpu));
			Assert::IsTrue(cpu_get_c_flag(&cpu));
		}

		TEST_METHOD(ADC_immediate_positive_number_overflow)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0x69;

			// immediate value
			cpu.memory.data[0x8001] = 0b01111111;

			cpu_init(&cpu, 0x8000);
			cpu.a = 0b00000001;
			cpu_exec(&cpu, 0x69);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.a == 0b10000000);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsTrue(cpu_get_n_flag(&cpu));
			Assert::IsTrue(cpu_get_v_flag(&cpu));
			Assert::IsFalse(cpu_get_c_flag(&cpu));
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
			nes.cpu.memory.data[0x8000] = 0x69;

			// immediate value
			nes.cpu.memory.data[0x8001] = -127;

			cpu_init(&nes.cpu, 0x8000);
			nes.cpu.a = -127;
			cpu_exec(&nes.cpu, 0x69);

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

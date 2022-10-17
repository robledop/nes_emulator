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
	TEST_CLASS(jsr_instruction_tests)
	{
	public:
		TEST_METHOD(JSR_absolute)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x20;

			// jump address 0x2211
			nes.cpu.memory.data[0x8001] = 0x11;
			nes.cpu.memory.data[0x8002] = 0x22;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0x20);

			Assert::IsTrue(nes.cpu.sp == 0xFD);
			Assert::IsTrue(nes.cpu.pc == 0x2211);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_c_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_i_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_v_flag(&nes.cpu));
			Assert::IsTrue(cpu_stack_pop_16(&nes.cpu) == 0x8003);
		}
	};
}

#pragma warning( pop ) 

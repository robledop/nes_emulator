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
	TEST_CLASS(plp_instruction_tests)
	{
	public:
		TEST_METHOD(PLP_implicit)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0x28;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.p = 0x11;
			cpu_stack_push_8(&nes.cpu, 0xFF);

			cpu_exec(&nes.cpu, 0x28);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8001);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_c_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_i_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_v_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_b_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_d_flag(&nes.cpu));
		}
	};
}

#pragma warning( pop ) 

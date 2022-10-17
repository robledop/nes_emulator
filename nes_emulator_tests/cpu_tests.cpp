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
	TEST_CLASS(cpu_tests)
	{
	public:

		TEST_METHOD(cpu_init_test)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			cpu_init(&nes.cpu, 0x8000);

			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.x == 0x00);
			Assert::IsTrue(nes.cpu.y == 0x00);
			Assert::IsTrue(nes.cpu.pc == 0x8000);
		}

		TEST_METHOD(cpu_clear_memory_test)
		{
			cpu cpu;
			cpu_clear_memory(&cpu);

			for (auto i : cpu.memory.data)
			{
				Assert::IsTrue(cpu.memory.data[i] == 0x00);
			}
		}
		
	};
}

#pragma warning( pop ) 

#include "pch.h"

#include "CppUnitTest.h"
extern "C" {
#include "../nes_emulator/cpu.h"
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
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			cpu_init(&cpu);

			Assert::IsTrue(cpu.A == 0x00);
			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.X == 0x00);
			Assert::IsTrue(cpu.Y == 0x00);
			Assert::IsTrue(cpu.PC == 0x8000);
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

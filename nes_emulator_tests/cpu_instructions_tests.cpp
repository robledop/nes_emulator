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
	TEST_CLASS(cpu_instructions_tests)
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

		TEST_METHOD(LDA_immediate_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA9;

			// immediate value
			cpu.memory.data[0x8001] = 0x11;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA9);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0x11);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_immediate_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA9;

			// immediate value
			cpu.memory.data[0x8001] = -10;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA9);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0xF6);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsTrue(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_immediate_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA9;

			// immediate value
			cpu.memory.data[0x8001] = 0x00;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA9);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0x00);
			Assert::IsTrue(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}


		TEST_METHOD(LDA_zero_page_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA5;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0055] = 0x11;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA5);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0x11);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_zero_page_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA5;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0055] = -10;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA5);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0xF6);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsTrue(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_zero_page_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA5;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0055] = 0x00;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA5);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0x00);
			Assert::IsTrue(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}


		TEST_METHOD(LDA_zero_page_x_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xB5;

			// value
			cpu.memory.data[0x8001] = 0x55;

			
			cpu.memory.data[0x0065] = 0x11;

			cpu_init(&cpu);

			cpu.X = 0x10;

			cpu_exec(&cpu, 0xB5);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0x11);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_zero_page_x_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xB5;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0065] = -10;

			cpu_init(&cpu);

			cpu.X = 0x10;
			
			cpu_exec(&cpu, 0xB5);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0xF6);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsTrue(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_zero_page_x_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xB5;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0065] = 0x00;

			cpu_init(&cpu);
			
			cpu.X = 0x10;

			cpu_exec(&cpu, 0xB5);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8001);
			Assert::IsTrue(cpu.A == 0x00);
			Assert::IsTrue(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_zero_page_absolute_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xAD;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5501] = 0x11;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xAD);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8002);
			Assert::IsTrue(cpu.A == 0x11);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_zero_page_absolute_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xAD;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5501] = -10;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xAD);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8002);
			Assert::IsTrue(cpu.A == 0xF6);
			Assert::IsFalse(cpu_get_Z_flag(&cpu));
			Assert::IsTrue(cpu_get_N_flag(&cpu));
		}

		TEST_METHOD(LDA_zero_page_absolute_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xAD;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5501] = 0x00;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xAD);

			Assert::IsTrue(cpu.SP == 0xFF);
			Assert::IsTrue(cpu.PC == 0x8002);
			Assert::IsTrue(cpu.A == 0x00);
			Assert::IsTrue(cpu_get_Z_flag(&cpu));
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}
	};
}

#pragma warning( pop ) 

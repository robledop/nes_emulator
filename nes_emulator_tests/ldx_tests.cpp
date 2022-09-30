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
	TEST_CLASS(ldx_instruction_tests)
	{
	public:

		TEST_METHOD(LDX_immediate_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA2;

			// immediate value
			cpu.memory.data[0x8001] = 0x11;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA2);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_immediate_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA2;

			// immediate value
			cpu.memory.data[0x8001] = -10;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA2);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsTrue(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_immediate_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA2;

			// immediate value
			cpu.memory.data[0x8001] = 0x00;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA2);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}


		TEST_METHOD(LDX_zero_page_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA6;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0055] = 0x11;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA6);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_zero_page_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA6;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0055] = -10;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xA6);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsTrue(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_zero_page_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xA6;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0055] = 0x00;


			cpu_init(&cpu);
			cpu.x = 0x01;
			cpu_exec(&cpu, 0xA6);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}


		TEST_METHOD(LDX_zero_page_y_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xB6;

			// value
			cpu.memory.data[0x8001] = 0x55;


			cpu.memory.data[0x0065] = 0x11;

			cpu_init(&cpu);

			cpu.y = 0x10;
			cpu.x = 0x01;

			cpu_exec(&cpu, 0xB6);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_zero_page_y_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xB6;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0065] = -10;

			cpu_init(&cpu);

			cpu.y = 0x10;
			cpu.x = 0x01;

			cpu_exec(&cpu, 0xB6);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsTrue(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_zero_page_y_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xB6;

			// pointer
			cpu.memory.data[0x8001] = 0x55;

			// value
			cpu.memory.data[0x0065] = 0x00;

			cpu_init(&cpu);

			cpu.y = 0x10;
			cpu.x = 0x01;

			cpu_exec(&cpu, 0xB6);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8001);
			Assert::IsTrue(cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_absolute_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xAE;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5501] = 0x11;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xAE);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8002);
			Assert::IsTrue(cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_absolute_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xAE;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5501] = -10;

			cpu_init(&cpu);
			cpu_exec(&cpu, 0xAE);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8002);
			Assert::IsTrue(cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsTrue(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_absolute_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xAE;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5501] = 0x00;

			cpu_init(&cpu);
			cpu.x = 0x01;
			cpu_exec(&cpu, 0xAE);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8002);
			Assert::IsTrue(cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_absolute_y_positive_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xBE;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5511] = 0x11;

			cpu_init(&cpu);

			cpu.y = 0x10;

			cpu_exec(&cpu, 0xBE);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8002);
			Assert::IsTrue(cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_absolute_y_negative_number)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xBE;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5511] = -10;

			cpu_init(&cpu);

			cpu.y = 0x10;

			cpu_exec(&cpu, 0xBE);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8002);
			Assert::IsTrue(cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&cpu));
			Assert::IsTrue(cpu_get_n_flag(&cpu));
		}

		TEST_METHOD(LDX_absolute_y_zero)
		{
			cpu cpu;

			// reset vector
			cpu.memory.data[0xFFFC] = 0x00;
			cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			cpu.memory.data[0x8000] = 0xBE;

			// low byte
			cpu.memory.data[0x8001] = 0x01;
			// high byte
			cpu.memory.data[0x8002] = 0x55;
			// value
			cpu.memory.data[0x5511] = 0x00;

			cpu_init(&cpu);

			cpu.y = 0x10;
			cpu.x = 0x01;

			cpu_exec(&cpu, 0xBE);

			Assert::IsTrue(cpu.sp == 0xFF);
			Assert::IsTrue(cpu.pc == 0x8002);
			Assert::IsTrue(cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&cpu));
			Assert::IsFalse(cpu_get_n_flag(&cpu));
		}
	};
}

#pragma warning( pop ) 

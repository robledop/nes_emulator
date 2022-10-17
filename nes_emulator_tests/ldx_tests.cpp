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
	TEST_CLASS(ldx_instruction_tests)
	{
	public:

		TEST_METHOD(LDX_immediate_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA2;

			// immediate value
			nes.cpu.memory.data[0x8001] = 0x11;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA2);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_immediate_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA2;

			// immediate value
			nes.cpu.memory.data[0x8001] = -10;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA2);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_immediate_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA2;

			// immediate value
			nes.cpu.memory.data[0x8001] = 0x00;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA2);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}


		TEST_METHOD(LDX_zero_page_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA6;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0055] = 0x11;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA6);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_zero_page_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA6;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0055] = -10;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA6);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_zero_page_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA6;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0055] = 0x00;


			cpu_init(&nes.cpu, 0x8000);
			nes.cpu.x = 0x01;
			cpu_exec(&nes.cpu, 0xA6);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}


		TEST_METHOD(LDX_zero_page_y_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB6;

			// value
			nes.cpu.memory.data[0x8001] = 0x55;


			nes.cpu.memory.data[0x0065] = 0x11;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;
			nes.cpu.x = 0x01;

			cpu_exec(&nes.cpu, 0xB6);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_zero_page_y_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB6;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0065] = -10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;
			nes.cpu.x = 0x01;

			cpu_exec(&nes.cpu, 0xB6);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_zero_page_y_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB6;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0065] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;
			nes.cpu.x = 0x01;

			cpu_exec(&nes.cpu, 0xB6);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_absolute_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xAE;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5501] = 0x11;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xAE);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_absolute_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xAE;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5501] = -10;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xAE);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_absolute_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xAE;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5501] = 0x00;

			cpu_init(&nes.cpu, 0x8000);
			nes.cpu.x = 0x01;
			cpu_exec(&nes.cpu, 0xAE);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_absolute_y_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xBE;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = 0x11;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xBE);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.x == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_absolute_y_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xBE;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = -10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xBE);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.x == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDX_absolute_y_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xBE;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;
			nes.cpu.x = 0x01;

			cpu_exec(&nes.cpu, 0xBE);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.x == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}
	};
}

#pragma warning( pop ) 

#include "pch.h"
#include <format>
#include <iostream>

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
	TEST_CLASS(lda_instruction_tests)
	{
	public:

		TEST_METHOD(LDA_immediate_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA9;

			// immediate value
			nes.cpu.memory.data[0x8001] = 0x11;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_immediate_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA9;

			// immediate value
			nes.cpu.memory.data[0x8001] = -10;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_immediate_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA9;

			// immediate value
			nes.cpu.memory.data[0x8001] = 0x00;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}


		TEST_METHOD(LDA_zero_page_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA5;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0055] = 0x11;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA5);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_zero_page_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA5;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0055] = -10;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA5);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_zero_page_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA5;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0055] = 0x00;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xA5);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}


		TEST_METHOD(LDA_zero_page_x_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB5;

			// value
			nes.cpu.memory.data[0x8001] = 0x55;


			nes.cpu.memory.data[0x0065] = 0x11;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xB5);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_zero_page_x_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB5;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0065] = -10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xB5);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_zero_page_x_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB5;

			// pointer
			nes.cpu.memory.data[0x8001] = 0x55;

			// value
			nes.cpu.memory.data[0x0065] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xB5);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_absolute_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xAD;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5501] = 0x11;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xAD);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003, L"Actual: ");
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_absolute_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xAD;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5501] = -10;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xAD);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_absolute_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xAD;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5501] = 0x00;

			cpu_init(&nes.cpu, 0x8000);
			cpu_exec(&nes.cpu, 0xAD);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}



		TEST_METHOD(LDA_absolute_x_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xBD;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = 0x11;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xBD);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_absolute_x_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xBD;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = -10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xBD);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_absolute_x_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xBD;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xBD);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}


		TEST_METHOD(LDA_absolute_y_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB9;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = 0x11;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xB9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_absolute_y_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB9;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = -10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xB9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_absolute_y_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB9;

			// low byte
			nes.cpu.memory.data[0x8001] = 0x01;
			// high byte
			nes.cpu.memory.data[0x8002] = 0x55;
			// value
			nes.cpu.memory.data[0x5511] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xB9);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8003);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}


		TEST_METHOD(LDA_indirect_x_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA1;

			// first part of the vector
			nes.cpu.memory.data[0x8001] = 0x02;

			// low byte
			nes.cpu.memory.data[0x12] = 0x11;
			// high byte
			nes.cpu.memory.data[0x13] = 0x01;

			// value
			nes.cpu.memory.data[0x0111] = 0x11;

			cpu_init(&nes.cpu, 0x8000);

			// will be added to the vector
			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xA1);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_indirect_x_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA1;

			// first part of the vector
			nes.cpu.memory.data[0x8001] = 0x02;

			// low byte
			nes.cpu.memory.data[0x12] = 0x11;
			// high byte
			nes.cpu.memory.data[0x13] = 0x01;

			// value
			nes.cpu.memory.data[0x0111] = -10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xA1);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_indirect_x_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xA1;

			// first part of the vector
			nes.cpu.memory.data[0x8001] = 0x02;

			// low byte
			nes.cpu.memory.data[0x12] = 0x11;
			// high byte
			nes.cpu.memory.data[0x13] = 0x01;

			// value
			nes.cpu.memory.data[0x0111] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.x = 0x10;

			cpu_exec(&nes.cpu, 0xA1);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}


		TEST_METHOD(LDA_indirect_y_positive_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB1;

			// vector
			nes.cpu.memory.data[0x8001] = 0x02;

			// low byte of the address
			nes.cpu.memory.data[0x02] = 0x11;
			// high byte of the address
			nes.cpu.memory.data[0x03] = 0x01;

			// value
			nes.cpu.memory.data[0x0121] = 0x11;

			cpu_init(&nes.cpu, 0x8000);

			// will be added to the address
			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xB1);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x11);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_indirect_y_negative_number)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB1;

			// vector
			nes.cpu.memory.data[0x8001] = 0x02;

			// low byte of the address
			nes.cpu.memory.data[0x02] = 0x11;
			// high byte of the address
			nes.cpu.memory.data[0x03] = 0x01;

			// value
			nes.cpu.memory.data[0x0121] = -10;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xB1);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0xF6);
			Assert::IsFalse(cpu_get_z_flag(&nes.cpu));
			Assert::IsTrue(cpu_get_n_flag(&nes.cpu));
		}

		TEST_METHOD(LDA_indirect_y_zero)
		{
			nes nes;
			cpu_clear_memory(&nes.cpu);
			nes.cpu.controller = &nes.controller;

			// reset vector
			nes.cpu.memory.data[0xFFFC] = 0x00;
			nes.cpu.memory.data[0xFFFD] = 0x80;

			// instruction
			nes.cpu.memory.data[0x8000] = 0xB1;

			// vector
			nes.cpu.memory.data[0x8001] = 0x02;

			// low byte of the address
			nes.cpu.memory.data[0x02] = 0x11;
			// high byte of the address
			nes.cpu.memory.data[0x03] = 0x01;

			// value
			nes.cpu.memory.data[0x0121] = 0x00;

			cpu_init(&nes.cpu, 0x8000);

			nes.cpu.y = 0x10;

			cpu_exec(&nes.cpu, 0xB1);

			Assert::IsTrue(nes.cpu.sp == 0xFF);
			Assert::IsTrue(nes.cpu.pc == 0x8002);
			Assert::IsTrue(nes.cpu.a == 0x00);
			Assert::IsTrue(cpu_get_z_flag(&nes.cpu));
			Assert::IsFalse(cpu_get_n_flag(&nes.cpu));
		}
	};
}

#pragma warning( pop ) 

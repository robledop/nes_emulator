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
	TEST_CLASS(cpu_flags_tests)
	{
	public:

		TEST_METHOD(cpu_set_C_flag_test)
		{
			cpu cpu;
			cpu.P = 0b11111110;
			cpu_set_C_flag(&cpu, 1);

			Assert::IsTrue(cpu.P == 0b11111111);
			cpu_set_C_flag(&cpu, 0);

			Assert::IsTrue(cpu.P == 0b11111110);
		}

		TEST_METHOD(cpu_get_C_flag_test)
		{
			cpu cpu;
			cpu.P = 0b11111111;
			cpu_get_C_flag(&cpu);

			Assert::IsTrue(cpu_get_C_flag(&cpu));

			cpu.P = 0b11111110;
			cpu_get_C_flag(&cpu);

			Assert::IsFalse(cpu_get_C_flag(&cpu));
		}

		TEST_METHOD(cpu_set_Z_flag_test)
		{
			cpu cpu;
			cpu.P = 0b11111101;
			cpu_set_Z_flag(&cpu, 1);

			Assert::IsTrue(cpu.P == 0b11111111);

			cpu_set_Z_flag(&cpu, 0);

			Assert::IsTrue(cpu.P == 0b11111101);
		}

		TEST_METHOD(cpu_get_Z_flag_test)
		{
			cpu cpu;
			cpu.P = 0b00000010;

			Assert::IsTrue(cpu_get_Z_flag(&cpu));

			cpu.P = 0b11111101;

			Assert::IsFalse(cpu_get_Z_flag(&cpu));
		}

		TEST_METHOD(cpu_set_I_flag_test)
		{
			cpu cpu;
			cpu.P = 0b11111011;
			cpu_set_I_flag(&cpu, 1);

			Assert::IsTrue(cpu.P == 0b11111111);

			cpu_set_I_flag(&cpu, 0);

			Assert::IsTrue(cpu.P == 0b11111011);
		}

		TEST_METHOD(cpu_get_I_flag_test)
		{
			cpu cpu;
			cpu.P = 0b00000100;

			Assert::IsTrue(cpu_get_I_flag(&cpu));

			cpu.P = 0b11111011;

			Assert::IsFalse(cpu_get_I_flag(&cpu));
		}

		TEST_METHOD(cpu_set_D_flag_test)
		{
			cpu cpu;
			cpu.P = 0b11110111;
			cpu_set_D_flag(&cpu, 1);

			Assert::IsTrue(cpu.P == 0b11111111);

			cpu_set_D_flag(&cpu, 0);

			Assert::IsTrue(cpu.P == 0b11110111);
		}

		TEST_METHOD(cpu_get_D_flag_test)
		{
			cpu cpu;

			cpu.P = 0b00001000;
			Assert::IsTrue(cpu_get_D_flag(&cpu));

			cpu.P = 0b11110111;
			Assert::IsFalse(cpu_get_D_flag(&cpu));
		}

		TEST_METHOD(cpu_set_B_flag_test)
		{
			cpu cpu;
			cpu.P = 0b11101111;
			cpu_set_B_flag(&cpu, 1);

			Assert::IsTrue(cpu.P == 0b11111111);

			cpu_set_B_flag(&cpu, 0);

			Assert::IsTrue(cpu.P == 0b11101111);
		}

		TEST_METHOD(cpu_get_B_flag_test)
		{
			cpu cpu;
			cpu.P = 0b00010000;
			Assert::IsTrue(cpu_get_B_flag(&cpu));

			cpu.P = 0b11101111;
			Assert::IsFalse(cpu_get_B_flag(&cpu));
		}

		TEST_METHOD(cpu_set_V_flag_test)
		{
			cpu cpu;
			cpu.P = 0b10111111;
			cpu_set_V_flag(&cpu, 1);

			Assert::IsTrue(cpu.P == 0b11111111);

			cpu_set_V_flag(&cpu, 0);

			Assert::IsTrue(cpu.P == 0b10111111);
		}

		TEST_METHOD(cpu_get_V_flag_test)
		{
			cpu cpu;

			cpu.P = 0b00100000;
			Assert::IsTrue(cpu_get_V_flag(&cpu));

			cpu.P = 0b01011111;
			Assert::IsFalse(cpu_get_V_flag(&cpu));
		}

		TEST_METHOD(cpu_set_N_flag_test)
		{
			cpu cpu;
			cpu.P = 0b01111111;
			cpu_set_N_flag(&cpu, 1);

			Assert::IsTrue(cpu.P == 0b11111111);

			cpu_set_N_flag(&cpu, 0);

			Assert::IsTrue(cpu.P == 0b01111111);
		}

		TEST_METHOD(cpu_get_N_flag_test)
		{
			cpu cpu;

			cpu.P = 0b10000000;
			Assert::IsTrue(cpu_get_N_flag(&cpu));

			cpu.P = 0b01111111;
			Assert::IsFalse(cpu_get_N_flag(&cpu));
		}
	};
}

#pragma warning( pop ) 

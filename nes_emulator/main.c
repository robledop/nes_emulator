#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

#include "cpu.h"

int load_file(char** text, const char* filename, uint32_t* size_out);

void print_header_info(const char* rom)
{
	printf("%s:", "Header");
	for (int i = 0; i < 4; ++i)
	{
		putchar(rom[i]);
	}
	puts("");
	printf("PRG-ROM size: %d bytes\n", rom[4] * 1024 * 16);
	printf("CHR-ROM size: %d bytes\n", rom[5] * 1024 * 8);

	const byte flags_6 = rom[6];
	if (flags_6 & 0b00000001)
	{
		puts("Mirroring: horizontal (vertical arrangement) (CIRAM A10 = PPU A11)");
	}
	else
	{
		puts("Mirroring: vertical (horizontal arrangement) (CIRAM A10 = PPU A10)");
	}

	if (flags_6 & 0b00000010)
	{
		puts("Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory");
	}

	if (flags_6 & 0b00000100)
	{
		puts("512-byte trainer at $7000-$71FF (stored before PRG data)");
	}

	if (flags_6 & 0b00001000)
	{
		puts("Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM");
	}

	const byte mapper_low_nibble = flags_6 & 0b11110000;

	const byte flags_7 = rom[7];

	printf("VS Unisystem: %d\n", flags_7 & 0b00000001);
	printf("PlayChoice-10: %d\n", flags_7 & 0b00000010);
	if (flags_7 & 0b00001000)
	{
		puts("flags 8-15 are in NES 2.0 format");
	}

	const byte mapper_high_nibble = flags_7 & 0b11110000;
	const byte mapper = mapper_low_nibble | mapper_high_nibble;

	printf("Mapper: %x\n", mapper);
	printf("PRG-RAM size: %d bytes\n", rom[8] * 1024 * 8);
	printf("TV system: %s\n", rom[9] & 0b00000001 ? "PAL" : "NTSC");

	puts("flags 10:");
	const byte flags_10 = rom[10];

	switch (flags_10 & 0b00000011)
	{
	case 0:
		puts("TV system: NTSC");
		break;
	case 2:
		puts("TV system: PAL");
		break;
	default:
		puts("TV system: dual compatible");
		break;
	}
}

int main(const int argc, char** argv)
{
	char* rom = NULL;
	uint32_t size = 0;

	const int result = load_file(&rom, argv[1], &size);

	if (result != 0 || rom == NULL)
	{
		puts("There was an error while trying to load the file.");
		free(rom);
		return result;
	}

	cpu cpu;
	cpu_clear_memory(&cpu);

	print_header_info(rom);


	memcpy(&cpu.memory.data[0x8000], &rom[16], 0x8000);

	cpu_init(&cpu);
	while (true)
	{
		cpu_exec(&cpu, cpu.memory.data[cpu.pc]);
	}

	free(rom);

	return 0;
}


int load_file(char** text, const char* filename, uint32_t* size_out)
{
	FILE* fp = fopen(filename, "r");
	if (fp != NULL)
	{
		if (fseek(fp, 0L, SEEK_END) == 0)
		{
			// Get the size of the file.
			const long bufsize = ftell(fp);
			if (bufsize == -1)
			{
				return 1;
			}
			*size_out = sizeof(char) * (bufsize + 1) + 100;
			*text = malloc(*size_out);

			// Go back to the start of the file.
			if (fseek(fp, 0L, SEEK_SET) != 0)
			{
				return 1;
			}

			// Read the entire file into memory.
			fread(*text, sizeof(char), bufsize, fp);
			if (ferror(fp) != 0)
			{
				fputs("Error reading file", stderr);
			}
		}
		fclose(fp);
		return 0;
	}
	return 1;
}
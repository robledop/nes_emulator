#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

#include "cpu.h"

int load_file(char** text, const char* filename, uint32_t* size_out);

int main (const int argc, char **argv)
{
	char* rom = NULL;
	uint32_t size = 0;

	const int result = load_file(&rom, argv[1], &size);

	if (result != 0)
	{
		puts("There was an error while trying to load the file.");
		free(rom);
		return result;
	}

	cpu cpu;

	memcpy(&cpu.memory.data[0x8000], rom, size);

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
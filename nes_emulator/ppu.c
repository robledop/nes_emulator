#include <assert.h>
#include "ppu.h"

word get_background_palette(const word attribute) {
	switch (attribute) {
		case 0:
			return 0x3F00;
		case 1:
			return 0x3F04;
		case 2:
			return 0x3F08;
		case 3:
			return 0x3F0C;
		default:
			assert(false);
			return 0;
	}
}

void get_rgb_color(Uint8* r, Uint8* g, Uint8* b, word index)
{
	const uint32_t colors = ppu_colors[index];
	*r = (colors >> 16) & 0xFF;
	*g = (colors >> 8) & 0xFF;
	*b = (colors) & 0xFF;
}

void draw_tile_row(SDL_Renderer* renderer, const byte lo_byte, byte hi_byte, int x, int y, word attribute, ppu* ppu)
{

	int rx = x;
	for (int i = 0; i < 8; i++)
	{
		const bool is_lo_set = lo_byte & (0b10000000 >> i);
		const bool is_hi_set = hi_byte & (0b10000000 >> i);

		Uint8 red;
		Uint8 green;
		Uint8 blue;
		if (is_lo_set && !is_hi_set)
		{
			const word palette_base = get_background_palette((attribute >> 6) & 0b00000011);
			const word value = ppu->memory.data[palette_base + 2];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}
		else if (is_lo_set && is_hi_set)
		{
			const word palette_base = get_background_palette((attribute >> 4) & 0b00000011);
			const word value = ppu->memory.data[palette_base + 3];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}
		else if (!is_lo_set && is_hi_set)
		{
			const word palette_base = get_background_palette((attribute >> 2) & 0b00000011);
			const word value = ppu->memory.data[palette_base + 1];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}
		else if (!is_lo_set && !is_hi_set)
		{
			const word palette_base = get_background_palette(attribute & 0b00000011);
			const word value = ppu->memory.data[palette_base + 0];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}

		SDL_Rect r;
		r.x = rx;
		r.y = y;
		r.w = PIXEL_HEIGHT;
		r.h = PIXEL_HEIGHT;
		SDL_RenderFillRect(renderer, &r);
		rx += PIXEL_HEIGHT;
	}
}

void draw_tile(SDL_Renderer* renderer, ppu* ppu, int x, int y, const word pattern_pos, word attr_tb_addr)
{
	// There's certainly a better way to do this, but here we go
	const int tx = x / 16;
	const int ty = y / 16;
	int offset = 0;
	if (ty >= 0x00 && ty <= 0x03)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 0;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 1;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 2;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 3;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 4;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 5;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 6;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 7;
		}
	}
	else if (ty > 0x03 && ty <= 0x07)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 8;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 9;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 10;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 11;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 12;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 13;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 14;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 15;
		}
	}
	else if (ty > 0x04 && ty <= 0x0b)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 16;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 17;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 18;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 19;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 20;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 21;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 22;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 23;
		}
	}
	else if (ty > 0x0b && ty <= 0x0f)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 24;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 25;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 26;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 27;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 28;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 29;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 30;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 31;
		}
	}
	else if (ty > 0x0f && ty <= 0x13)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 32;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 33;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 34;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 35;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 36;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 37;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 38;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 39;
		}
	}
	else if (ty > 0x13 && ty <= 0x17)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 40;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 41;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 42;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 43;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 44;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 45;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 46;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 47;
		}
	}
	else if (ty > 0x17 && ty <= 0x1b)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 48;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 49;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 50;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 51;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 52;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 53;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 54;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 55;
		}
	}
	else if (ty > 0x1b && ty <= 0x1d)
	{
		if (tx >= 0x00 && tx <= 0x03)
		{
			offset = 56;
		}
		else if (tx > 0x03 && tx <= 0x07)
		{
			offset = 57;
		}
		else if (tx > 0x07 && tx <= 0x0b)
		{
			offset = 58;
		}
		else if (tx > 0x0b && tx <= 0x0f)
		{
			offset = 59;
		}
		else if (tx > 0x0f && tx <= 0x13)
		{
			offset = 60;
		}
		else if (tx > 0x13 && tx <= 0x17)
		{
			offset = 61;
		}
		else if (tx > 0x17 && tx <= 0x1b)
		{
			offset = 62;
		}
		else if (tx > 0x1b && tx <= 0x1f)
		{
			offset = 63;
		}
	}



	const byte attribute = ppu->memory.data[attr_tb_addr + offset];

	for (word z = 0; z < 8; z++)
	{
		const byte tile_hi_byte = ppu->memory.data[pattern_pos + z];
		const byte tile_lo_byte = ppu->memory.data[pattern_pos + z + 8];


		draw_tile_row(renderer, tile_lo_byte, tile_hi_byte, x, y, attribute, ppu);
		y += PIXEL_HEIGHT;
	}
}

void draw_tiles(SDL_Renderer* renderer, const ppu* ppu)
{
	int x = 0;
	int y = 0;

	word bg_pattern_table_addr = 0;
	if (ppu->registers.ppu_ctrl & BG_PT_ADDR_FLAG)
	{
		bg_pattern_table_addr = PATTERN_TABLE_1;
	}
	else
	{
		bg_pattern_table_addr = PATTERN_TABLE_0;
	}

	word name_table_address = 0;

	switch (ppu->registers.ppu_ctrl & NAME_TABLE_ADDR_FLAGS)
	{
		case 0:
			name_table_address = NAME_TABLE_0;
			break;
		case 1:
			name_table_address = NAME_TABLE_1;
			break;
		case 2:
			name_table_address = NAME_TABLE_2;
			break;
		case 3:
			name_table_address = NAME_TABLE_3;
			break;
	}

	const word attribute_table_address = name_table_address + 960;

	for (word i = 0; i < NAME_TABLE_SIZE; i++)
	{
		const word name_table_pos = name_table_address + i;
		const word tile_index = ppu->memory.data[name_table_pos];

		const word pattern_pos = bg_pattern_table_addr + (tile_index * 16);

		draw_tile(renderer, ppu, x, y, pattern_pos, attribute_table_address);

		if (x >= 245 * PIXEL_HEIGHT)
		{
			y += 8 * PIXEL_HEIGHT;
			x = 0;
		}
		else
		{
			x += 8 * PIXEL_HEIGHT;
		}
	}
}

void render_background(const ppu* ppu, SDL_Renderer* renderer, SDL_Window* window)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				SDL_DestroyWindow(window);

		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);


	draw_tiles(renderer, ppu);

	SDL_RenderPresent(renderer);
}
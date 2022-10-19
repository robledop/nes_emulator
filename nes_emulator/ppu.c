#include <assert.h>
#include "ppu.h"

word get_background_palette(const word attribute)
{
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

word get_sprite_palette(const word attribute)
{
	switch (attribute) {
		case 0:
			return 0x3F10;
		case 1:
			return 0x3F14;
		case 2:
			return 0x3F18;
		case 3:
			return 0x3F1C;
		default:
			assert(false);
			return 0;
	}
}

void get_rgb_color(Uint8* r, Uint8* g, Uint8* b, const word index)
{
	const uint32_t colors = ppu_colors[index];
	*r = (colors >> 16) & 0xFF;
	*g = (colors >> 8) & 0xFF;
	*b = colors & 0xFF;
}

void draw_bg_tile_row(const ppu* ppu, SDL_Renderer* renderer, const byte lo_byte, const byte hi_byte, const int x, const int y, const word palette_base)
{
	int rx = x;
	for (int i = 0; i < 8; i++)
	{
		const bool is_lo_set = lo_byte & (0b10000000 >> i);
		const bool is_hi_set = hi_byte & (0b10000000 >> i);

		Uint8 red;
		Uint8 green;
		Uint8 blue;

		if (!is_hi_set && !is_lo_set) // 0
		{
			const word value = ppu->memory.data[palette_base + 0];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}
		else if (!is_hi_set && is_lo_set) // 1
		{
			const word value = ppu->memory.data[palette_base + 2];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}
		else if (is_hi_set && !is_lo_set) // 2
		{
			const word value = ppu->memory.data[palette_base + 1];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}
		else if (is_hi_set && is_lo_set) // 3
		{
			const word value = ppu->memory.data[palette_base + 3];
			get_rgb_color(&red, &green, &blue, value);
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		}

		SDL_Rect r;
		r.x = rx;
		r.y = y;
		r.w = PIXEL_WIDTH;
		r.h = PIXEL_HEIGHT;
		SDL_RenderFillRect(renderer, &r);
		rx += PIXEL_WIDTH;
	}
}

void draw_sprite_row_pixel(const ppu* ppu, SDL_Renderer* renderer, const byte lo_byte, const byte hi_byte, const int y, int* x, const word attribute, const int bit)
{
	const bool is_lo_set = lo_byte & (0b10000000 >> bit);
	const bool is_hi_set = hi_byte & (0b10000000 >> bit);

	Uint8 red;
	Uint8 green;
	Uint8 blue;
	const word palette_base = get_sprite_palette(attribute & 0b00000011);

	if (!is_hi_set && is_lo_set) // 1
	{
		const word index = ppu->memory.data[palette_base + 2];
		get_rgb_color(&red, &green, &blue, index);
		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}
	else if (is_hi_set && !is_lo_set) // 2
	{
		const word index = ppu->memory.data[palette_base + 1];
		get_rgb_color(&red, &green, &blue, index);
		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}
	else if (is_hi_set && is_lo_set) // 3
	{
		const word index = ppu->memory.data[palette_base + 3];
		get_rgb_color(&red, &green, &blue, index);
		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}

	SDL_Rect r;
	r.x = *x;
	r.y = y;
	r.w = PIXEL_WIDTH;
	r.h = PIXEL_HEIGHT;
	if (is_hi_set || is_lo_set)
	{
		SDL_RenderFillRect(renderer, &r);
	}
	*x += PIXEL_WIDTH;
}

void draw_sprite_tile_row(const ppu* ppu, SDL_Renderer* renderer, const byte lo_byte, const byte hi_byte, const int x, const int y, const word attribute)
{
	const bool flip_horizontally = attribute & 0b01000000;

	int rx = x;

	if (flip_horizontally)
	{
		for (int i = 7; i > -1; i--)
		{
			draw_sprite_row_pixel(ppu, renderer, lo_byte, hi_byte, y, &rx, attribute, i);
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			draw_sprite_row_pixel(ppu, renderer, lo_byte, hi_byte, y, &rx, attribute, i);
		}
	}
}

void draw_bg_tile(const ppu* ppu, SDL_Renderer* renderer, int x, int y, const word pattern_pos, const word attr_tb_addr, const word nt_pos)
{
	// https://www.nesdev.org/wiki/PPU_scrolling#Tile_and_attribute_fetching
	const word attribute_address = attr_tb_addr | (nt_pos & 0x0C00) | ((nt_pos >> 4) & 0x38) | ((nt_pos >> 2) & 0x07);

	const byte attribute = ppu->memory.data[attribute_address];

	const byte attribute_shift = ((nt_pos & 0x40) >> 4) | (nt_pos & 0x2);
	const byte palette_selector = (attribute >> attribute_shift) & 0x3;


	const word palette_base = get_background_palette(palette_selector);

	for (word i = 0; i < 8; i++)
	{
		const byte tile_hi_byte = ppu->memory.data[pattern_pos + i];
		const byte tile_lo_byte = ppu->memory.data[pattern_pos + i + 8];

		draw_bg_tile_row(ppu, renderer, tile_lo_byte, tile_hi_byte, x, y, palette_base);
		y += PIXEL_HEIGHT;
	}
}

word get_name_table(const ppu* ppu)
{
	switch (ppu->registers.ppu_ctrl & NAME_TABLE_ADDR_FLAGS)
	{
		case 0:
			return NAME_TABLE_0;
		case 1:
			return  NAME_TABLE_1;
		case 2:
			return NAME_TABLE_2;
		case 3:
			return  NAME_TABLE_3;
		default:
			assert(false);
			return 0;
	}
}

word get_pattern_table(const ppu* ppu)
{
	if (ppu->registers.ppu_ctrl & BG_PT_ADDR_FLAG)
	{
		return PATTERN_TABLE_1;
	}
	else
	{
		return PATTERN_TABLE_0;
	}
}

void draw_tiles(const ppu* ppu, SDL_Renderer* renderer)
{
	int x = 0;
	int y = 0;

	const word bg_pattern_table_addr = get_pattern_table(ppu);
	const word name_table_address = get_name_table(ppu);

	const word attribute_table_address = name_table_address + 960;

	for (word i = 0; i < NAME_TABLE_SIZE; i++)
	{
		const word name_table_pos = name_table_address + i;
		const word tile_index = ppu->memory.data[name_table_pos];

		const word pattern_pos = bg_pattern_table_addr + (tile_index * 16);

		draw_bg_tile(ppu, renderer, x * (TILE_WIDTH * PIXEL_WIDTH), y * (TILE_HEIGHT * PIXEL_HEIGHT), pattern_pos, attribute_table_address, name_table_address + i);

		if (x >= 31)
		{
			y += 1;
			x = 0;
		}
		else
		{
			x += 1;
		}
	}
}

void draw_sprite_tile(const ppu* ppu, SDL_Renderer* renderer, word x, word y, word tile_index, byte attributes)
{
	const bool flip_vertically = attributes & 0b10000000;

	if(flip_vertically)
	{
		for (int i = 7; i > -1; i--)
		{
			const byte tile_hi_byte = ppu->memory.data[tile_index + i];
			const byte tile_lo_byte = ppu->memory.data[tile_index + i + 8];

			draw_sprite_tile_row(ppu, renderer, tile_lo_byte, tile_hi_byte, x, y, attributes);
			y += PIXEL_HEIGHT;
		}
	}
	else
	{
		for (word i = 0; i < 8; i++)
		{
			const byte tile_hi_byte = ppu->memory.data[tile_index + i];
			const byte tile_lo_byte = ppu->memory.data[tile_index + i + 8];

			draw_sprite_tile_row(ppu, renderer, tile_lo_byte, tile_hi_byte, x, y, attributes);
			y += PIXEL_HEIGHT;
		}
	}
}

void draw_sprites(const ppu* ppu, SDL_Renderer* renderer)
{
	word sprite_pattern_table_addr;
	if (ppu->registers.ppu_ctrl & SPRITE_PT_ADDR_FLAG)
	{
		sprite_pattern_table_addr = PATTERN_TABLE_1;
	}
	else
	{
		sprite_pattern_table_addr = PATTERN_TABLE_0;
	}

	for (int i = 0; i < 256; i += 4)
	{
		const byte sprite_y = ppu->oam.data[i];
		const word sprite_tile_index = sprite_pattern_table_addr + (word)(ppu->oam.data[i + 1] << 4);
		const byte sprite_attributes = ppu->oam.data[i + 2];
		const byte sprite_x = ppu->oam.data[i + 3];

		draw_sprite_tile(ppu, renderer, (word)(sprite_x * PIXEL_WIDTH), (word)(sprite_y * PIXEL_HEIGHT), sprite_tile_index, sprite_attributes);
	}
}

void render_background(const ppu* ppu, SDL_Renderer* renderer)
{
	draw_tiles(ppu, renderer);
}

void render_sprites(const ppu* ppu, SDL_Renderer* renderer)
{
	draw_sprites(ppu, renderer);
	SDL_RenderPresent(renderer);
}
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

void get_rgb_color(Uint8* r, Uint8* g, Uint8* b, const word index)
{
	const uint32_t colors = ppu_colors[index];
	*r = (colors >> 16) & 0xFF;
	*g = (colors >> 8) & 0xFF;
	*b = colors & 0xFF;
}

void draw_tile_row(SDL_Renderer* renderer, const byte lo_byte, const byte hi_byte, const int x, const int y, const word attribute, const ppu* ppu)
{

	int rx = x;
	for (int i = 0; i < 8; i++)
	{
		const bool is_lo_set = lo_byte & (0b10000000 >> i);
		const bool is_hi_set = hi_byte & (0b10000000 >> i);

		// MONOCHROME MODE

		//if (is_hi_set || is_lo_set)
		//{
		//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//}
		//else
		//{
		//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//}

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
		r.w = PIXEL_WIDTH;
		r.h = PIXEL_HEIGHT;
		SDL_RenderFillRect(renderer, &r);
		rx += PIXEL_WIDTH;
	}
}

void draw_bg_tile(SDL_Renderer* renderer, const ppu* ppu, int x, int y, const word pattern_pos, const word attr_tb_addr, const word nt_pos)
{
	// https://www.nesdev.org/wiki/PPU_scrolling#Tile_and_attribute_fetching
	const word attribute_address = attr_tb_addr | (nt_pos & 0x0C00) | ((nt_pos >> 4) & 0x38) | ((nt_pos >> 2) & 0x07);

	const byte attribute = ppu->memory.data[attribute_address];

	for (word i = 0; i < 8; i++)
	{
		const byte tile_hi_byte = ppu->memory.data[pattern_pos + i];
		const byte tile_lo_byte = ppu->memory.data[pattern_pos + i + 8];


		draw_tile_row(renderer, tile_lo_byte, tile_hi_byte, x, y, attribute, ppu);
		y += PIXEL_HEIGHT;
	}
}

void draw_tiles(SDL_Renderer* renderer, const ppu* ppu)
{
	int x = 0;
	int y = 0;

	word bg_pattern_table_addr;
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
		default:
			assert(false);
	}

	const word attribute_table_address = name_table_address + 960;

	for (word i = 0; i < NAME_TABLE_SIZE; i++)
	{
		const word name_table_pos = name_table_address + i;
		const word tile_index = ppu->memory.data[name_table_pos];

		const word pattern_pos = bg_pattern_table_addr + (tile_index * 16);

		draw_bg_tile(renderer, ppu, x, y, pattern_pos, attribute_table_address, name_table_address + i);

		if (x >= (SCREEN_WIDTH * PIXEL_WIDTH))
		{
			y += (TILE_HEIGHT * PIXEL_HEIGHT);
			x = 0;
		}
		else
		{
			x += (TILE_WIDTH * PIXEL_WIDTH);
		}
	}
}

void draw_sprite_tile(SDL_Renderer* renderer, const ppu* ppu, int x, int y, byte tile_index, byte attributes)
{
	word index = tile_index << 4;
	for (word i = 0; i < 8; i++)
	{
		const byte tile_hi_byte = ppu->memory.data[index + i];
		const byte tile_lo_byte = ppu->memory.data[index + i + 8];

		draw_tile_row(renderer, tile_lo_byte, tile_hi_byte, x, y, attributes, ppu);
		y += PIXEL_HEIGHT;
	}
}

void draw_sprites(SDL_Renderer* renderer, const ppu* ppu)
{
	for (int i = 0; i < 32; i += 4)
	{
		const byte sprite_x = ppu->oam.data[i];
		const byte sprite_tile_index = ppu->oam.data[i + 1];
		const byte sprite_attributes = ppu->oam.data[i + 2];
		const byte sprite_y = ppu->oam.data[i + 3];

		draw_sprite_tile(renderer, ppu, sprite_x + (8 * PIXEL_WIDTH), sprite_y + (8 * PIXEL_HEIGHT), sprite_tile_index, sprite_attributes);
	}

}

void render_background(const ppu* ppu, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	draw_tiles(renderer, ppu);

	SDL_RenderPresent(renderer);
}

void render_sprites(const ppu* ppu, SDL_Renderer* renderer)
{
	draw_sprites(renderer, ppu);

	SDL_RenderPresent(renderer);
}
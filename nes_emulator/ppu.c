#include "ppu.h"


void draw_tile_row(SDL_Renderer* renderer, const byte row, int x, int y)
{
	int rx = x;
	for (int i = 0; i < 8; i++)
	{
		const bool is_set = row & ( 0b10000000 >> i);
		if (is_set)
		{
			SDL_Rect r;
			r.x = rx;
			r.y = y;
			r.w = PIXEL_HEIGHT;
			r.h = PIXEL_HEIGHT;
			SDL_RenderFillRect(renderer, &r);
		}
		rx += PIXEL_HEIGHT;
	}
}

void draw_tile(SDL_Renderer* renderer, const ppu* ppu, int x, int y, const word pattern_pos)
{
	for (word z = 0; z < 8; z++)
	{
		const byte tile_hi_byte = ppu->memory.data[pattern_pos + z];
		const byte tile_lo_byte = ppu->memory.data[pattern_pos + z + 8];

		const byte tile_row = tile_hi_byte | tile_lo_byte;

		draw_tile_row(renderer, tile_row, x, y);
		y += PIXEL_HEIGHT;
	}
}

void draw_tiles(SDL_Renderer* renderer, const ppu* ppu)
{
	int x = 0;
	int y = 0;
	for (word i = 0; i < NAME_TABLE_SIZE; i++)
	{
		const word name_table_pos = NAME_TABLE_0 + i;
		const word tile_index = ppu->memory.data[name_table_pos];

		const word pattern_pos = PATTERN_TABLE_1 + (tile_index * 16);

		draw_tile(renderer, ppu, x, y, pattern_pos);

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

void render_background(const ppu* ppu)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(
		EMULATOR_WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH * PIXEL_HEIGHT,
		SCREEN_HEIGHT * PIXEL_WIDTH,
		SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

	while (1)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					goto out;

			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

		draw_tiles(renderer, ppu);

		SDL_RenderPresent(renderer);
	}

out:
	SDL_DestroyWindow(window);
}
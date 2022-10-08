#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "SDL.h"

#include "config.h"

#define VRAM_SIZE 0x3FFF

typedef struct
{
	byte data[VRAM_SIZE];
} vram;

typedef struct
{
	byte ppu_ctrl;
	byte ppu_mask;
	byte ppu_status;
	byte oam_addr;
	byte oam_data;
	byte ppu_scroll;
	byte ppu_addr;
	byte ppu_data;
	byte oam_dma;


} registers;


typedef struct
{
	vram memory;
	registers registers;
	bool ppu_data_latch;
	word ppu_data_addr;
} ppu;

#define SCREEN_HEIGHT		225
#define SCREEN_WIDTH		256

#define TILE_HEIGHT			8
#define TILE_WIDTH			8

#define PIXEL_HEIGHT		2
#define PIXEL_WIDTH			2

#define PATTERN_TABLE_0		0
#define PATTERN_TABLE_1		0x1000

#define PATTERN_TABLE_1		0x1000
#define NAME_TABLE_0		0x2000
#define NAME_TABLE_1		0x2400
#define NAME_TABLE_2		0x2800
#define NAME_TABLE_3		0x2C00

#define PATTERN_TABLE_SIZE	0x1000
#define NAME_TABLE_SIZE		0x0400

// Background pattern table address (0: $0000; 1: $1000)
#define BG_PT_ADDR_FLAG		0b00010000

// Base name table address
// (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
#define NAME_TABLE_ADDR_FLAGS 0b00000011

#define PALETTE_BASE		  0X3F00

static const uint32_t ppu_colors[64] =
{
	0x757575, 0x271B8F, 0x0000AB, 0x47009F, 0x8F0077, 0xAB0013, 0xA70000, 0x7F0B00,
	0x432F00, 0x004700, 0x005100, 0x003F17, 0x1B3F5F, 0x000000, 0x000000, 0x000000,
	0xBCBCBC, 0x0073EF, 0x233BEF, 0x8300F3, 0xBF00BF, 0xE7005B, 0xDB2B00, 0xCB4F0F,
	0x8B7300, 0x009700, 0x00AB00, 0x00933B, 0x00838B, 0x000000, 0x000000, 0x000000,
	0xFFFFFF, 0x3FBFFF, 0x5F97FF, 0xA78BFD, 0xF77BFF, 0xFF77B7, 0xFF7763, 0xFF9B3B,
	0xF3BF3F, 0x83D313, 0x4FDF4B, 0x58F898, 0x00EBDB, 0x000000, 0x000000, 0x000000,
	0xFFFFFF, 0xABE7FF, 0xC7D7FF, 0xD7CBFF, 0xFFC7FF, 0xFFC7DB, 0xFFBFB3, 0xFFDBAB,
	0xFFE7A3, 0xE3FFA3, 0xABF3BF, 0xB3FFCF, 0x9FFFF3, 0x000000, 0x000000, 0x000000
};

void render_background(const ppu* ppu, SDL_Renderer* renderer, SDL_Window* window);
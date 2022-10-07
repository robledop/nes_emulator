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

void render_background(const ppu* ppu);
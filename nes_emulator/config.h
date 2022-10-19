#pragma once

typedef unsigned char byte;
typedef  unsigned short word;
typedef enum address_mode
{
	implicit,
	accumulator,
	immediate,
	zero_page,
	zero_page_x,
	zero_page_y,
	relative,
	absolute,
	absolute_x,
	absolute_y,
	indirect,
	// (Indirect,X)
	indexed_indirect,
	// (Indirect),Y
	indirect_indexed
} address_mode;

#define PPU_CTRL		0x2000
#define PPU_MASK		0x2001
#define PPU_STATUS		0x2002
#define OAM_ADDR		0x2003
#define OAM_DATA		0x2004
#define PPU_SCROLL		0x2005
#define PPU_ADDR		0x2006
#define PPU_DATA		0x2007

#define OAM_DMA			0x4014

#define CONTROLLER_1	0x4016
#define CONTROLLER_2	0x4017

#define EMULATOR_WINDOW_TITLE "NES Emulator"


//#define LOGGING

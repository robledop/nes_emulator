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
	indexed_indirect,
	indirect_indexed
} address_mode;

#define PPU_CTRL	0x2000
#define PPU_MASK	0x2001
#define PPU_STATUS	0x2002
#define OAM_ADDR	0x2003
#define OAM_DATA	0x2004
#define PPU_SCROLL	0x2005
#define PPU_ADDR	0x2006
#define PPU_DATA	0x2007

#define EMULATOR_WINDOW_TITLE "NES"
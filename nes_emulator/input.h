#pragma once
#include <SDL.h>
#include <stdbool.h>

#include "config.h"

typedef struct
{
	bool latch;
	byte current_read;
	byte buttons[8];
} controller;

void init_controller(controller *controller);
byte read_next_button(controller *controller);
void handle_input(controller* controller, const SDL_Event* event);
void write_controller(controller* controller, const byte value);

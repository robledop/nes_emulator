#include "input.h"

byte read_next_button(controller* controller)
{
	const byte button_value = controller->buttons[controller->current_read++];
	controller->current_read = controller->current_read % 8;
	return button_value;
}

void write_controller(controller *controller, const byte value)
{
	if (controller->latch && value == 0)
	{
		controller->current_read = 0;
	}

	controller->latch = value;
}

void init_controller(controller* controller)
{
	controller->current_read = 0;
	for (int i = 0; i < 8; ++i)
	{
		controller->buttons[i] = 0;
	}
}

void handle_input(controller* controller, const SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.sym) {
			case SDLK_z:
				controller->buttons[0] = 1; // A
				break;
			case SDLK_x:
				controller->buttons[1] = 1; // B
				break;
			case SDLK_SPACE:
				controller->buttons[2] = 1; // Select
				break;
			case SDLK_RETURN:
				controller->buttons[3] = 1; // Start
				break;
			case SDLK_UP:
				controller->buttons[4] = 1; // Up
				break;
			case SDLK_DOWN:
				controller->buttons[5] = 1; // Down
				break;
			case SDLK_LEFT:
				controller->buttons[6] = 1; // Left
				break;
			case SDLK_RIGHT:
				controller->buttons[7] = 1; // Right
				break;
			default:
				break;
		}
	}
	else if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym) {
			case SDLK_z:
				controller->buttons[0] = 0; // A
				break;
			case SDLK_x:
				controller->buttons[1] = 0; // B
				break;
			case SDLK_SPACE:
				controller->buttons[2] = 0; // Select
				break;
			case SDLK_RETURN:
				controller->buttons[3] = 0; // Start 
				break;
			case SDLK_UP:
				controller->buttons[4] = 0; // Up
				break;
			case SDLK_DOWN:
				controller->buttons[5] = 0; // Down
				break;
			case SDLK_LEFT:
				controller->buttons[6] = 0; // Left
				break;
			case SDLK_RIGHT:
				controller->buttons[7] = 0; // Right
				break;
			default:
				break;
		}
	}
}
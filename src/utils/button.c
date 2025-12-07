/*
*	Contributor:
*
*	Axel:
*		- Added button handling
*
*	Zimon:
*		- Moved initial press logic from game to here
*/

#include "utils/button.h"

#define BUTTONS_ADDRESS 0x040000d0

volatile struct buttons {
	uint32_t data;
	uint32_t direction;
	uint32_t interruptmask;
	uint32_t edgecapture;
	uint32_t outset;
	uint32_t outclear;
} * buttons = (void *) BUTTONS_ADDRESS;

char button_pressed = 0;

uint32_t buttons_get_states() {
    return (buttons->data);
}

char button_get_is_initial_press() {
	const char new_button_state = buttons->data & 0x1;
	const char ret = !button_pressed && new_button_state;
	button_pressed = new_button_state;
	return ret;
}

#include "utils/buttons.h"

#define BUTTONS_ADDRESS 0x04000010
#define BUTTON_COUNT 12

volatile struct buttons {
	uint32_t data;
	uint32_t direction;
	uint32_t interruptmask;
	uint32_t edgecapture;
	uint32_t outset;
	uint32_t outclear;
} * buttons = (void *) BUTTONS_ADDRESS;

static uint32_t buttons_data;

uint32_t buttons_get_button_state(uint32_t button) {
	return (buttons_data >> button) & 0x1;
}

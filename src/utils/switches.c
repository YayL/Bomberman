#include "utils/switches.h"

#define SWITCH_ADDRESS 0x04000010
#define SWITCH_COUNT 10

volatile struct switches {
	uint32_t data;
	uint32_t direction;
	uint32_t interruptmask;
	uint32_t edgecapture;
	uint32_t outset;
	uint32_t outclear;
} * switches = (void *) SWITCH_ADDRESS;

static uint32_t switches_data;

uint32_t switches_get_switch_state(uint32_t switch_nr) {
	return (switches->data >> switch_nr) & 0x1;
}

uint32_t switches_get_all_switch_states() {
	return switches->data & ((1 << 10) - 1);
}

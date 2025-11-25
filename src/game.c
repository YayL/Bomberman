#include "game.h"
#include "dtekv-lib.h"
#include "utils/timer.h"

static char is_running = 1;

void game_set_running_state(char state) {
	is_running = state;
}

void game_init() {
	game_set_running_state(1);

	volatile char *VGA = (volatile char*)0x08000000;

    for (int i = 0; i < 320*240; i++) {
        VGA[i] = 80; 
	}
}

void game_run() {
	while (is_running) {
		uint32_t delta = timer_get_delta_us();

		puts("Delta: ");
		print_dec(delta);
		puts("us\n");
	}
}

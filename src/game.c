#include "game.h"
#include "dtekv-lib.h"
#include "utils/timer.h"
#include "utils/screen.h"
#include "player.h"


static char is_running = 1;

void game_set_running_state(char state) {
	is_running = state;
}

void game_init() {
	fill_background();
	draw_border();
	draw_inner_squares();
	player_draw();
}

void game_run() {
	while (is_running) {

		// uint32_t delta = timer_get_delta_us();
		//
		// puts("Delta: ");
		// print_dec(delta);
		// puts("us\n");
	}
}

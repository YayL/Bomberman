#include "game.h"
#include "dtekv-lib.h"
#include "utils/timer.h"
#include "utils/screen.h"
#include "utils/switches.h"
#include "utils/hw_counters.h"

#include "menus/start.h"
#include "menus/playing.h"
#include "menus/gameover.h"

static enum game_state current_state = GAME_STATE_NONE;

#define TARGET_FPS 60
#define TARGET_TIME_UPDATE (1e6 / TARGET_FPS) // 1e6 is 1 second

static char is_running = 1;
static uint32_t time_since_last_update = 0;

void game_set_is_running(char state) {
	is_running = state;
}

void game_set_game_state(enum game_state state) {
	switch (state) {
		case GAME_STATE_START:
			start_menu_init(); break;
		case GAME_STATE_PLAYING:
			playing_menu_init(); break;
		case GAME_STATE_GAMEOVER:
			gameover_menu_init(); break;
		default:
			puts("Tried setting game state to invalid active game state");
			exit();
	}

	current_state = state;
}

void game_init() {
	screen_init();
	game_set_game_state(GAME_STATE_START);
}

char prev_state = 0;
void game_step(uint32_t delta) {

	char new_state = switches_get_switch_state(4);
	if (!prev_state && new_state) {
		hw_counters_report();
	}
	prev_state = new_state;

	switch (current_state) {
		case GAME_STATE_START:
			start_menu_update();
			start_menu_draw();
			break;
		case GAME_STATE_PLAYING:
			playing_menu_update(time_since_last_update);
			playing_menu_draw(time_since_last_update);
			break;
		case GAME_STATE_GAMEOVER:
			gameover_menu_update();
			gameover_menu_draw();
			break;
		default:
			puts("Invalid game state in game loop");
			exit();
	}

	screen_blit();
}

void game_run() {
	time_since_last_update = 0;

	while (is_running) {
		uint32_t delta = timer_get_delta_us();
		time_since_last_update += delta;

		if (time_since_last_update < TARGET_TIME_UPDATE) {
			continue;
		}

		game_step(time_since_last_update);
		time_since_last_update = 0;

		#ifdef EMULATOR
		for (uint32_t i = 0; i < 50000; ++i) { // wait roughly 5ms
			__asm__("nop");
		}
		#endif
	}
}

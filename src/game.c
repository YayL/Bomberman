#include "game.h"
#include "dtekv-lib.h"
#include "utils/timer.h"
#include "utils/screen.h"
#include "player.h"
#include "utils/button.h"

#include "menus/start.h"
#include "menus/classic.h"

enum game_state {
	GAME_STATE_NONE,
	GAME_STATE_START,
	GAME_STATE_CLASSIC
};

static enum game_state current_state = GAME_STATE_NONE;
#define TARGET_FPS 60
#define TARGET_TIME_UPDATE (1e6 / TARGET_FPS)

static char is_running = 1;
static uint32_t time_since_last_update;

void game_set_running_state(char state) {
	is_running = state;
}

void game_set_game_state(enum game_state state) {
	switch (state) {
		case GAME_STATE_START:
			start_menu_init(); break;
		case GAME_STATE_CLASSIC:
			classic_menu_init(); break;
		default:
			puts("Tried setting game state to invalid active game state");
			exit();
	}

	current_state = state;
}

void game_init() {
	screen_init();
	game_set_game_state(GAME_STATE_CLASSIC);
}

void game_run() {
	char temp = 1;
	char button_down = 0;
	char previous_button_state = 0;

	time_since_last_update = 0;

	while (is_running) {
		if(button_get_state() && !previous_button_state){
			button_down = 1;
		} else {
			button_down = 0;
		}
		previous_button_state = button_get_state();

			case GAME_STATE_MENU:
				if (button_down) {
					currentState = GAME_STATE_PLAYING;
				}
		uint32_t delta = timer_get_delta_us();
		time_since_last_update += delta;

		if (time_since_last_update < TARGET_TIME_UPDATE) {
			continue;
		}

		switch (current_state) {
			case GAME_STATE_START:
				start_menu_update();
				start_menu_draw();
				break;
			case GAME_STATE_PLAYING:
				if (temp) {
					game_init();
					temp = 0;
				}
				player_update();
				player_draw();
				draw_world();
				if (button_down) {
					mark_bomb_at_player_position();
				}
			case GAME_STATE_CLASSIC:
				classic_menu_update(time_since_last_update);
				classic_menu_draw(time_since_last_update);
				break;
			default:
				puts("Invalid game state in game loop");
				exit();
		}

		screen_blit();
		time_since_last_update = 0;
	}
}

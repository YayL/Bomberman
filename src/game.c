#include "game.h"
#include "dtekv-lib.h"
#include "utils/timer.h"
#include "utils/screen.h"
#include "player.h"

#define LIGHT_GREY RGB(7, 7, 7)
#define GREY RGB(4, 4, 2)
#define BLACK RGB(0, 0, 0)
#define WHITE RGB(7, 7, 3)
#define YELLOW RGB(7, 5, 0)
#define BACKGROUND_GREEN RGB(0, 4, 0)

typedef enum{
	GAME_STATE_MENU,
	GAME_STATE_PLAYING
} game_state;

static game_state currentState = GAME_STATE_PLAYING;

static char is_running = 1;

void game_set_running_state(char state) {
	is_running = state;
}

void game_init() {
	switch (currentState) {
		case GAME_STATE_MENU:
			fill_background(BLACK);
			draw_word(SCREEN_WIDTH / 2 - 40, 10, "BOMBERMAN", YELLOW);
			draw_word(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 4, "START", WHITE);
			break;
		case GAME_STATE_PLAYING:
			fill_background(BACKGROUND_GREEN);
			draw_border();
			draw_inner_squares();
			draw_word(SCREEN_WIDTH / 2 - 40, 10, "BOMBERMAN", LIGHT_GREY);
			break;
	}
}

void game_run() {
	while (is_running) {
		switch (currentState) {
			case GAME_STATE_MENU:
				break;
			case GAME_STATE_PLAYING:
				player_update();
				player_draw();
				break;
		}
		// uint32_t delta = timer_get_delta_us();
		//
		// puts("Delta: ");
		// print_dec(delta);
		// puts("us\n");
	}
}

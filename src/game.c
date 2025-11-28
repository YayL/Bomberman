#include "game.h"
#include "dtekv-lib.h"
#include "utils/timer.h"
#include "utils/screen.h"
#include "player.h"
#include "utils/button.h"

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

static game_state currentState = GAME_STATE_MENU;

static char is_running = 1;

void game_set_running_state(char state) {
	is_running = state;
}

void game_init() {
	switch (currentState) {
		case GAME_STATE_MENU:
			fill_background(BLACK);
			draw_text(SCREEN_WIDTH / 2 - 36, 10, "BOMBERMAN", YELLOW);
			draw_text(SCREEN_WIDTH / 2 - 84, SCREEN_HEIGHT / 2 - 20, "PRESS BUTTON TO START", WHITE);
			draw_text(10, SCREEN_HEIGHT - 60, "UP:    SWITCH 0", WHITE);
			draw_text(10, SCREEN_HEIGHT - 50, "DOWN:  SWITCH 1", WHITE);
			draw_text(10, SCREEN_HEIGHT - 40, "RIGHT: SWITCH 8", WHITE);
			draw_text(10, SCREEN_HEIGHT - 30, "LEFT:  SWITCH 9", WHITE);
			draw_text(10, SCREEN_HEIGHT - 20, "BOMB:  BUTTON", WHITE);
			break;
		case GAME_STATE_PLAYING:
			fill_background(BACKGROUND_GREEN);
			mark_world();
			draw_text(SCREEN_WIDTH / 2 - 36, 10, "BOMBERMAN", LIGHT_GREY);
			break;
	}
}

void game_run() {
	char temp = 1;
	char button_down = 0;
	char previous_button_state = 0;

	while (is_running) {
		if(button_get_state() && !previous_button_state){
			button_down = 1;
		} else {
			button_down = 0;
		}
		previous_button_state = button_get_state();

		switch (currentState) {
			case GAME_STATE_MENU:
				if (button_down) {
					currentState = GAME_STATE_PLAYING;
				}
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
				break;
		}
		// uint32_t delta = timer_get_delta_us();
		//
		// puts("Delta: ");
		// print_dec(delta);
		// puts("us\n");
	}
}

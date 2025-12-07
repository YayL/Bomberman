/*
	Contributors: Axel
*/

#include "menus/start.h"

#include "utils/screen.h"
#include "utils/button.h"
#include "game.h"

void start_menu_init() {

}

void start_menu_update() {
	if (button_get_is_initial_press()) {
		game_set_game_state(GAME_STATE_PLAYING);
	}
}

void start_menu_draw() {
	fill_background(BLACK);
	draw_text(SCREEN_WIDTH / 2 - 40, 10, "BOMBERMAN", YELLOW);
	draw_text(SCREEN_WIDTH / 2 - 72, SCREEN_HEIGHT / 2 - 4, "PRESS BTN TO START", WHITE);

	draw_text(10, SCREEN_HEIGHT - 60, "UP:    SWITCH 1", WHITE);
	draw_text(10, SCREEN_HEIGHT - 50, "DOWN:  SWITCH 8", WHITE);
	draw_text(10, SCREEN_HEIGHT - 40, "RIGHT: SWITCH 0", WHITE);
	draw_text(10, SCREEN_HEIGHT - 30, "LEFT:  SWITCH 9", WHITE);
	draw_text(10, SCREEN_HEIGHT - 20, "BOMB:  BUTTON", WHITE);
}

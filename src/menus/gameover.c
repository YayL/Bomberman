/*
Contributors: Axel
*/

#include "menus/gameover.h"

#include "utils/screen.h"
#include "utils/button.h"
#include "game.h"

void gameover_menu_init() {

}

void gameover_menu_update() {
	if (button_get_is_initial_press()) {
		game_set_game_state(GAME_STATE_PLAYING);
	}
}

void gameover_menu_draw() {
	fill_background(BLACK);
	draw_text(SCREEN_WIDTH / 2 - 36, 10, "GAME OVER", RED);
	draw_text(SCREEN_WIDTH / 2 - 92, SCREEN_HEIGHT / 2 - 4, "PRESS BTN TO PLAY AGAIN", WHITE);
}
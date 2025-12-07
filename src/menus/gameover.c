/*
*	Contributors: Axel
*/

#include "menus/gameover.h"
#include "menus/playing/enemies.h"

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
	if (enemies_get_alive_count()) {
		draw_text(SCREEN_WIDTH / 2 - 36, 10, "GAME OVER", RED);
	} else {
		draw_text(SCREEN_WIDTH / 2 - 34, 10, "YOU WON", YELLOW);
	}
	draw_text(SCREEN_WIDTH / 2 - 92, SCREEN_HEIGHT / 2 - 4, "PRESS BTN TO PLAY AGAIN", WHITE);
}

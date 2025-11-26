#include "menus/start.h"

#include "utils/screen.h"

void start_menu_init() {

}

void start_menu_update() {

}

void start_menu_draw() {
	fill_background(BLACK);
	draw_word(SCREEN_WIDTH / 2 - 40, 10, "BOMBERMAN", YELLOW);
	draw_word(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 4, "START", WHITE);
}

/*
*   Contiributors: Axel & Zimon
*
*   Axel:
*       - Initial implementation of drawing and updating
*
*   Zimon:
*       - FPS display
*       - Initialization
*/

#include "menus/playing.h"

#include "utils/screen.h"
#include "menus/playing/map.h"
#include "menus/playing/player.h"
#include "menus/playing/bomb.h"

void draw_fps(uint32_t delta);

void playing_menu_init() {
    player_init();
    map_init();
    bombs_init();
}

void playing_menu_update(uint32_t delta) {
	player_update(delta);
    map_update(delta);
}

char FPS_TEXT[8] = {'F', 'P', 'S', ':', ' ', '6', '0', 0};

void playing_menu_draw(uint32_t delta) {
    fill_background(BACKGROUND_GREEN);
    map_draw();

	draw_text(SCREEN_WIDTH / 2 - 40, 10, "BOMBERMAN", LIGHT_GREY);
    draw_fps(delta);

	player_draw();
}

void draw_fps(uint32_t delta) {
    uint32_t fps = 1e6 / delta;
    FPS_TEXT[5] = ((fps / 10) % 10) + '0';
    FPS_TEXT[6] = (fps % 10) + '0';
    draw_text(SCREEN_WIDTH - 60, 10, FPS_TEXT, LIGHT_GREY);
}

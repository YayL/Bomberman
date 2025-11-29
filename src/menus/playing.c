#include "menus/playing.h"

#include "common.h"

#include "player.h"
#include "utils/screen.h"

#include "wall_texture.h"

void draw_border();
void draw_fps(uint32_t delta);
void draw_inner_squares();

void playing_menu_init() {
}

void playing_menu_update(uint32_t delta) {
	player_update();
}

char FPS_TEXT[8] = {'F', 'P', 'S', ':', ' ', '6', '0', 0};

void playing_menu_draw(uint32_t delta) {
    fill_background(BACKGROUND_GREEN);
	draw_border();
	draw_inner_squares();

	draw_word(SCREEN_WIDTH / 2 - 40, 10, "BOMBERMAN", LIGHT_GREY);
    draw_fps(delta);

	player_draw();
}

void draw_fps(uint32_t delta) {
    uint32_t fps = 1e6 / delta;
    FPS_TEXT[5] = ((fps / 10) % 10) + '0';
    FPS_TEXT[6] = (fps % 10) + '0';
    draw_word(SCREEN_WIDTH - 60, 10, FPS_TEXT, LIGHT_GREY);
}

void draw_border() {
    draw_rectangle(0, 0, SCREEN_WIDTH, BLOCK_SIZE * 2, GREY);

    for (int x = 0; x < SCREEN_WIDTH; x += BLOCK_SIZE) {
        draw_texture(x, 32, wall_texture);
        draw_texture(x, SCREEN_HEIGHT - BLOCK_SIZE, wall_texture);
    }

    for (int y = 32; y < SCREEN_HEIGHT; y += BLOCK_SIZE) {
        draw_texture(0, y, wall_texture);
        draw_texture(SCREEN_WIDTH - BLOCK_SIZE, y, wall_texture);
    }
}

void draw_inner_squares() {
    for (int x = 32; x < (SCREEN_WIDTH / 2); x += 32) {
        for (int y = 64; y < SCREEN_HEIGHT - 32; y += 32) {
            draw_texture(x, y, wall_texture);
        }
    }

    for (int x = (SCREEN_WIDTH / 2) + 16; x < SCREEN_WIDTH - 32; x += 32) {
        for (int y = 64; y < SCREEN_HEIGHT - 32; y += 32) {
            draw_texture(x, y, wall_texture);
        }
    }
}

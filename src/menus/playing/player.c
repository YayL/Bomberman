#include "menus/playing/player.h"

#include "utils/screen.h"
#include "utils/switches.h"
#include "utils/button.h"
#include "game.h"

#include "menus/playing/bomb.h"

#include "assets/player_texture.h"

static struct player player = {
	.speed = 1,
	.position = { 
		.x = GRID_X_TO_SCREEN(1),
		.y = GRID_Y_TO_SCREEN(3)
	}
};


void player_init(){
	player.position.x = GRID_X_TO_SCREEN(1);
	player.position.y = GRID_Y_TO_SCREEN(3);
	player.speed = 1;
}

void player_update(uint32_t delta) {
	if (button_get_is_initial_press()) {
		bomb_place(
			SCREEN_X_TO_GRID(player.position.x + PLAYER_WIDTH / 2),
			SCREEN_Y_TO_GRID(player.position.y + PLAYER_HEIGHT / 2)
		);
	}

	const uint32_t switches = switches_get_all_switch_states();
	int32_t dx = 0, dy = 0;

	// Right
	if (CHECK_NTH_SWITCH(switches, 0)) {
		dx += 1;
	}

	// Up
	if (CHECK_NTH_SWITCH(switches, 1)) {
		dy -= 1;
	}

	// Down
	if (CHECK_NTH_SWITCH(switches, 8)) {
		dy += 1;
	}

	// Left
	if (CHECK_NTH_SWITCH(switches, 9)) {
		dx -= 1;
	}

	entity_move(&player.position, player.speed, dx, dy, PLAYER_WIDTH, PLAYER_HEIGHT);
}

void player_draw() {
	volatile unsigned char * const framebuffer = screen_get_framebuffer_addr();
	const uint32_t player_x = player.position.x, player_y = player.position.y;

	for (uint32_t y = 0; y < BLOCK_SIZE; ++y) {
		for (uint32_t x = 0; x < BLOCK_SIZE; ++x) {
			if (PLAYER_TEXTURE[y * BLOCK_SIZE+ x] == BACKGROUND_GREEN) {
				continue;
			}

			framebuffer[SCREEN_TO_PIXEL(player_x + x, player_y + y)] = PLAYER_TEXTURE[y * BLOCK_SIZE + x];
		}
	}
}

void player_kill() {
	game_set_game_state(GAME_STATE_GAMEOVER);
}

struct position player_get_position() {
	return player.position;
}

struct position player_get_grid_position() {
	return (struct position) {
		.x = SCREEN_X_TO_GRID(player.position.x),
		.y = SCREEN_Y_TO_GRID(player.position.y)
	};
}

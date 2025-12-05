#include "menus/playing/player.h"

#include "utils/screen.h"
#include "utils/switches.h"
#include "utils/button.h"

#include "menus/playing/map.h"
#include "menus/playing/bomb.h"

#include "assets/player_texture.h"

static struct player player = {
	.speed = 1,
	.position = { 
		.x = GRID_X_TO_SCREEN(1),
		.y = GRID_Y_TO_SCREEN(3)
	}
};

#define IS_AVAILABLE(X, Y) map_is_empty(SCREEN_X_TO_GRID(X), SCREEN_Y_TO_GRID(Y))
#define CLAMP(VALUE, MIN, MAX) if (VALUE < MIN) { VALUE = MIN; } else if (MAX < VALUE) { VALUE = MAX; }
static inline void player_move(int32_t dx, int32_t dy) {
	int32_t next_x = player.position.x + dx * player.speed;
	int32_t next_y = player.position.y + dy * player.speed;

	// This should never be a worry but just in case
	CLAMP(next_x, 0, SCREEN_WIDTH - PLAYER_WIDTH);
	CLAMP(next_y, 0, SCREEN_HEIGHT - PLAYER_HEIGHT);

	// Since player is smaller than blocks we can just check the corners of the
	// player and see if they intersect with something

	// Unsure why the -1 are neccessary but they fixed an ¿issue?

	if (dx < 0) {
		const char top_left_dx = !IS_AVAILABLE(next_x, player.position.y);
		const char bottom_left_dx = !IS_AVAILABLE(next_x, player.position.y + PLAYER_HEIGHT - 1);
		if (top_left_dx || bottom_left_dx) {
			// Going left and top left or bottom left intersects
			next_x = GRID_X_TO_SCREEN((SCREEN_X_TO_GRID(next_x) + 1));
		}
	} else if (dx > 0) {
		const char top_right_dx = !IS_AVAILABLE(next_x + PLAYER_WIDTH - 1, player.position.y);
		const char bottom_right_dx = !IS_AVAILABLE(next_x + PLAYER_WIDTH - 1, player.position.y + PLAYER_HEIGHT - 1);
		if (top_right_dx || bottom_right_dx) {
			// Going right and top right or bottom right intersects
			next_x = GRID_X_TO_SCREEN(SCREEN_X_TO_GRID(next_x + PLAYER_WIDTH)) - PLAYER_WIDTH;
		}
	}


	if (dy < 0) {
		const char top_left_dy = !IS_AVAILABLE(player.position.x, next_y);
		const char top_right_dy = !IS_AVAILABLE(player.position.x + PLAYER_WIDTH - 1, next_y);
		if (top_left_dy || top_right_dy) {
			// Going up and top left or top right intersects
			next_y = GRID_Y_TO_SCREEN((SCREEN_Y_TO_GRID(next_y) + 1));
		}
	} else if (dy > 0) {
		const char bottom_left_dy = !IS_AVAILABLE(player.position.x, next_y + PLAYER_HEIGHT - 1);
		const char bottom_right_dy = !IS_AVAILABLE(player.position.x + PLAYER_WIDTH - 1, next_y + PLAYER_HEIGHT - 1);
		if (bottom_left_dy || bottom_right_dy) {
			// Going down and bottom left or bottom right intersects
			next_y = GRID_Y_TO_SCREEN(SCREEN_Y_TO_GRID(next_y + PLAYER_HEIGHT)) - PLAYER_HEIGHT;
		}
	}

	// Since we don't check dxdy it can start twiching in a block by going diagonally into it

	player.position.x = next_x;
	player.position.y = next_y;
}

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

	player_move(dx, dy);
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

struct position player_get_position() {
	return player.position;
}

struct position player_get_grid_position() {
	return (struct position) {
		.x = SCREEN_X_TO_GRID(player.position.x),
		.y = SCREEN_Y_TO_GRID(player.position.y)
	};
}

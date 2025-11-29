#include "player.h"
#include "utils/screen.h"
#include "utils/switches.h"
#include "utils/button.h"
#include "menus/playing/map.h"

static struct player player = {
	.speed = 1,
	.x = GRID_X_TO_SCREEN(1) + 5,
	.y = GRID_Y_TO_SCREEN(3) + 5
};

#define IS_AVAILABLE(X, Y) map_is_empty(SCREEN_X_TO_GRID(X), SCREEN_Y_TO_GRID(Y))
#define CLAMP(VALUE, MIN, MAX) if (VALUE < MIN) { VALUE = MIN; } else if (MAX < VALUE) { VALUE = MAX; }
inline void player_move(int32_t dx, int32_t dy) {
	int32_t next_x = player.x + dx * player.speed;
	int32_t next_y = player.y + dy * player.speed;

	// This should never be a worry but just in case
	CLAMP(next_x, 0, SCREEN_WIDTH - PLAYER_WIDTH);
	CLAMP(next_y, 0, SCREEN_HEIGHT - PLAYER_HEIGHT);

	// Since player is smaller than blocks we can just check the corners of the
	// player and see if they intersect with something

	// Unsure why the -1 are neccessary but they fixed an ¿issue?

	const char top_left_dx = !IS_AVAILABLE(next_x, player.y);
	const char top_right_dx = !IS_AVAILABLE(next_x + PLAYER_WIDTH - 1, player.y);
	const char bottom_left_dx = !IS_AVAILABLE(next_x, player.y + PLAYER_HEIGHT - 1);
	const char bottom_right_dx = !IS_AVAILABLE(next_x + PLAYER_WIDTH - 1, player.y + PLAYER_HEIGHT - 1);

	if (dx < 0 && (top_left_dx || bottom_left_dx)) {
		// Going left and top left or bottom left intersects
		next_x = GRID_X_TO_SCREEN((SCREEN_X_TO_GRID(next_x) + 1));
	} else if (dx > 0 && (top_right_dx || bottom_right_dx)) {
		// Going right and top right or bottom right intersects
		next_x = GRID_X_TO_SCREEN(SCREEN_X_TO_GRID(next_x + PLAYER_WIDTH)) - PLAYER_WIDTH;
	}

	const char top_left_dy = !IS_AVAILABLE(player.x, next_y);
	const char top_right_dy = !IS_AVAILABLE(player.x + PLAYER_WIDTH - 1, next_y);
	const char bottom_left_dy = !IS_AVAILABLE(player.x, next_y + PLAYER_HEIGHT - 1);
	const char bottom_right_dy = !IS_AVAILABLE(player.x + PLAYER_WIDTH - 1, next_y + PLAYER_HEIGHT - 1);

	if (dy < 0 && (top_left_dy || top_right_dy)) {
		// Going up and top left or top right intersects
		next_y = GRID_Y_TO_SCREEN((SCREEN_Y_TO_GRID(next_y) + 1));
	} else if (dy > 0 && (bottom_left_dy || bottom_right_dy)) {
		// Going down and bottom left or bottom right intersects
		next_y = GRID_Y_TO_SCREEN(SCREEN_Y_TO_GRID(next_y + PLAYER_HEIGHT)) - PLAYER_HEIGHT;
	}

	// Since we don't check dxdy it is most likely possible to glitch through a
	// block by going diagonally through it

	player.x = next_x;
	player.y = next_y;
}

void player_update() {
	if (button_get_is_initial_press()) {
		map_place_bomb(SCREEN_X_TO_GRID(player.x), SCREEN_Y_TO_GRID(player.y));
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

void mark_bomb_at_player_position() {
	int grid_x = SCREEN_X_TO_GRID(player.x);
	int grid_y = SCREEN_Y_TO_GRID(player.y);

	// mark_bomb(grid_x, grid_y);
}

void player_draw() {
	draw_rectangle(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, RGB(7, 0, 0));
}

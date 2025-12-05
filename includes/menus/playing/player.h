#pragma once
#include "common.h"
#include "utils/screen.h"

struct player {
	struct position position; // pixel coordinates
	uint32_t score;
	uint32_t speed;
};

#define PLAYER_HEIGHT 14
#define PLAYER_WIDTH  12

void player_update(uint32_t delta);
void player_draw();

void player_kill();
void player_init();

void mark_bomb_at_player_position();

struct position player_get_position();
struct position player_get_grid_position();

static inline void player_check_explosion_collision(uint32_t bx, uint32_t by) {
	struct position p_pos = player_get_position();
	const char vertical_explosion_collision = hitbox_collision_check(
													p_pos.x, p_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT,
													GRID_X_TO_SCREEN(bx), GRID_Y_TO_SCREEN(by - 1), BLOCK_SIZE, BLOCK_SIZE * 3
												);
	const char horizontal_explosion_collision = hitbox_collision_check(
													p_pos.x, p_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT,
													GRID_X_TO_SCREEN(bx - 1), GRID_Y_TO_SCREEN(by), BLOCK_SIZE * 3, BLOCK_SIZE
												);

	if (vertical_explosion_collision || horizontal_explosion_collision) {
		player_kill();
	}
}

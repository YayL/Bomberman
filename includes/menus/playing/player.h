#pragma once
#include <stdint.h>

struct player {
	struct player_position {
		uint32_t x, y; // Screen coordinates
	} position;
	uint32_t score;
	uint32_t speed;
};

#define PLAYER_HEIGHT 10
#define PLAYER_WIDTH  10

void player_update();
void player_draw();
void mark_bomb_at_player_position();

struct player_position player_get_position();

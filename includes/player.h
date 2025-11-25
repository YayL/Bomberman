#pragma once
#include <stdint.h>

struct player {
	uint32_t x, y;
	uint32_t score;
};

#define PLAYER_HEIGHT 10
#define PLAYER_WIDTH  10

void player_update();
void player_draw();

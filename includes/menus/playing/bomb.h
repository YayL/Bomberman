#pragma once
#include "common.h"

#define MAX_BOMB_COUNT 3

struct bomb {
	struct bomb_position {
		uint32_t x, y; // Grid coordinates
	} position;

	uint32_t time_left;

	enum bomb_state {
		BOMB_STATE_NONE,
		BOMB_STATE_PRIMED,
		BOMB_STATE_EXPLODED
	} state;
};

void bombs_init();

void bombs_update(uint32_t delta);
void bombs_draw();

void bomb_place(uint32_t x, uint32_t y);

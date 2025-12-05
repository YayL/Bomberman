#pragma once
#include "common.h"

#define ENEMY_WIDTH 14
#define ENEMY_HEIGHT 14

#define ENEMY_SPAWN_RATE_NUMERATOR 2
#define ENEMY_SPAWN_RATE_DENOMINATOR 32
#define ENEMY_SPEED 1

#define MAX_ENEMY_COUNT 3
#define ENEMY_SAFE_ZONE_WIDTH 5
#define ENEMY_SAFE_ZONE_HEIGHT 5

#define ENEMY_DIRECTION_CHANGE_TIMER 500000 // 0.5 seconds

struct enemy {
	struct position position;
	struct movement {
		int32_t dx, dy;
	} movement;
	uint32_t speed;
	uint32_t timer;
	char killed;
};

void enemies_add(uint32_t x, uint32_t y);
void enemies_clear();

void enemies_update(uint32_t delta);
void enemies_draw();

void enemies_check_explosion_collision(uint32_t bx, uint32_t by);

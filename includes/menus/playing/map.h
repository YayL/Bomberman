#pragma once

#include "common.h"

enum __attribute__ ((__packed__)) map_tiles {
	TILE_EMPTY,
	TILE_WALL,
	TILE_STONE
};

void map_init();
void map_draw();
void map_update(uint32_t delta);

void map_place_bomb(uint32_t x, uint32_t y);
char map_is_empty(uint32_t x, uint32_t y);
char map_is_stone(uint32_t x, uint32_t y);
void map_set_tile(uint32_t x, uint32_t y, enum map_tiles tile);
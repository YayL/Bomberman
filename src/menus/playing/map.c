#include "menus/playing/map.h"

#include "utils/screen.h"

#include "assets/wall_texture.h"
#include "assets/stone_texture.h"

static enum map_tiles map[GRID_HEIGHT][GRID_WIDTH];

#define GRID_X_START 0
#define GRID_X_END GRID_WIDTH
#define GRID_Y_START 2
#define GRID_Y_END GRID_HEIGHT

void map_init() {
	for (uint32_t x = GRID_X_START; x < GRID_X_END; ++x) {
		map[GRID_Y_START][x] = TILE_WALL;
		map[GRID_Y_END - 1][x] = TILE_WALL;
	}

	for (uint32_t y = GRID_Y_START + 1; y < GRID_Y_END - 1; ++y) {
		map[y][GRID_X_START] = TILE_WALL;
		map[y][GRID_X_END - 1] = TILE_WALL;
	}

	const uint32_t GRID_X_MIDDLE = (GRID_X_END - GRID_X_START) / 2;

	for (uint32_t y = GRID_Y_START + 2; y < GRID_Y_END - 2; y += 2) {
		for (uint32_t x = GRID_X_START + 2; x < GRID_X_MIDDLE; x += 2) {
			map[y][x] = TILE_WALL;
			map[y][GRID_X_END - x - 1] = TILE_WALL;
		}
	}
}

void map_draw() {
	for (uint32_t y = GRID_Y_START; y < GRID_Y_END; ++y) {
		for (uint32_t x = GRID_X_START; x < GRID_X_END; ++x) {
			switch (map[y][x]) {
				case TILE_EMPTY: break;
				case TILE_STONE:
					draw_texture(GRID_X_TO_SCREEN(x), GRID_Y_TO_SCREEN(y), STONE_TEXTURE);
					break;
				case TILE_WALL:
					draw_texture(GRID_X_TO_SCREEN(x), GRID_Y_TO_SCREEN(y), WALL_TEXTURE);
					break;
			}
		}
	}
}

void map_place_bomb(uint32_t x, uint32_t y) {
	if (map[y][x] == TILE_EMPTY) {
	}
}

char map_is_empty(uint32_t x, uint32_t y) {
	return map[y][x] == TILE_EMPTY;
}

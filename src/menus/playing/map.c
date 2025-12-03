#include "menus/playing/map.h"

#include "utils/screen.h"
#include "utils/random.h"

#include "assets/wall_texture.h"
#include "assets/stone_texture.h"

#include "menus/playing/player.h"
#include "menus/playing/bomb.h"

static enum map_tiles map[GRID_HEIGHT][GRID_WIDTH];

#define GRID_X_START 0
#define GRID_X_END GRID_WIDTH
#define GRID_Y_START 2
#define GRID_Y_END GRID_HEIGHT

#define STONE_SPAWN_RATE_NUMERATOR 16
#define STONE_SPAWN_RATE_DENOMINATOR 32

void map_init() {
	// Place top and bottom walls
	for (uint32_t x = GRID_X_START; x < GRID_X_END; ++x) {
		map[GRID_Y_START][x] = TILE_WALL;
		map[GRID_Y_END - 1][x] = TILE_WALL;
	}

	// Place left and right walls
	for (uint32_t y = GRID_Y_START + 1; y < GRID_Y_END - 1; ++y) {
		map[y][GRID_X_START] = TILE_WALL;
		map[y][GRID_X_END - 1] = TILE_WALL;
	}

	// Place wall blocks in the grid
	const uint32_t GRID_X_MIDDLE = (GRID_X_END - GRID_X_START) / 2;
	for (uint32_t y = GRID_Y_START + 2; y < GRID_Y_END - 2; y += 2) {
		for (uint32_t x = GRID_X_START + 2; x < GRID_X_MIDDLE; x += 2) {
			map[y][x] = TILE_WALL;
			map[y][GRID_X_END - x - 1] = TILE_WALL;
		}
	}

	// Seed random with timer
	random_seed(timer_get_delta_us_no_reset());

	// Place breakable stone blocks
	for (uint32_t y = GRID_Y_START; y < GRID_Y_END; ++y) {
		for (uint32_t x = GRID_X_START; x < GRID_X_END; ++x) {
			if (!map_is_empty(x, y)) {
				continue;
			}

			if (random_get_in_range(0, STONE_SPAWN_RATE_DENOMINATOR) <= STONE_SPAWN_RATE_NUMERATOR) {
				map[y][x] = TILE_STONE;
			}
		}
	}

	// Make sure that player start location is safe/escapable
	struct player_position pos = player_get_position();
	pos.x = SCREEN_X_TO_GRID(pos.x);
	pos.y = SCREEN_Y_TO_GRID(pos.y);

	if (pos.x != GRID_X_START + 1 || pos.y != GRID_Y_START + 1) {
		puts("Fix player safety!\n");
		exit();
	}

	map[pos.y][pos.x] = TILE_EMPTY;
	map[pos.y + 1][pos.x] = TILE_EMPTY;
	map[pos.y][pos.x + 1] = TILE_EMPTY;
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

	bombs_draw();
}

void map_update(uint32_t delta) {
	bombs_update(delta);
}

char map_is_stone(uint32_t x, uint32_t y) {
	ASSERT1(x < GRID_WIDTH);
	ASSERT1(y < GRID_HEIGHT);

	return map[y][x] == TILE_STONE;
}

char map_is_empty(uint32_t x, uint32_t y) {
	ASSERT1(x < GRID_WIDTH);
	ASSERT1(y < GRID_HEIGHT);

	return map[y][x] == TILE_EMPTY;
}

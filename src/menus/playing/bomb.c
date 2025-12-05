#include "menus/playing/bomb.h"

#include "game.h"
#include "menus/playing/map.h"
#include "menus/playing/player.h"

#include "utils/screen.h"
#include "assets/bomb_texture.h"
#include "assets/ex_center_texture.h"
#include "assets/ex_horizontal_texture.h"
#include "assets/ex_vertical_texture.h"

static uint32_t bombs_placed_count = 0;
static struct bomb bombs[MAX_BOMB_COUNT] = {0};

#define BOMB_EXPLODED_TIME_SHOWN 1500000 // 1.5 seconds
#define BOMB_PRIMED_TIME_SHOWN	 3000000 // 3 seconds

void bombs_remove(uint32_t remove_count) {
	ASSERT1(remove_count <= bombs_placed_count);
	bombs_placed_count -= remove_count;

	// Shift down bombs
	for (uint32_t i = 0; i < bombs_placed_count; ++i) {
		bombs[i] = bombs[i + remove_count];
	}
}

void bombs_init() {
	bombs_placed_count = 0;
}

// Return value: true if this bomb should be removed
static inline char bomb_update(uint32_t delta, struct bomb * bomb) {
	switch (bomb->state) {
		case BOMB_STATE_PRIMED: {
			bomb->time_left -= delta;
			// If overflown = has exploded
			if (bomb->time_left > BOMB_PRIMED_TIME_SHOWN) {
				bomb->state = BOMB_STATE_EXPLODED;
				bomb->time_left = BOMB_EXPLODED_TIME_SHOWN;
				break;
			}
		} break;
		case BOMB_STATE_EXPLODED: {
			bomb->time_left -= delta;
			// If overflown = has exploded
			if (bomb->time_left > BOMB_EXPLODED_TIME_SHOWN) {
				return 1;
			}

			struct player_position player_pos = player_get_position();
		} break;
		default:
			puts("bomb_update called with invalid bomb state");
			exit();
	}

	return 0;
}

void bombs_update(uint32_t delta) {
	uint32_t remove_count = 0;
	for (uint32_t i = 0; i < bombs_placed_count; ++i) {
		remove_count += bomb_update(delta, &bombs[i]);
	}

	if (remove_count > 0) {
		puts("Removing bomb\n");
		bombs_remove(remove_count);
	}
}

#define DRAW_BOMB_EX_TEXTURE(X, Y, TEXTURE) \
	if (map_is_empty(X, Y) || map_is_stone(X, Y)) { \
		draw_texture( \
			GRID_X_TO_SCREEN(X), \
			GRID_Y_TO_SCREEN(Y), \
			TEXTURE \
		); \
		map_set_tile(X, Y, TILE_EMPTY); \
	}


void draw_valid_tile(struct bomb *bomb) {
	uint32_t x = bomb->position.x;
	uint32_t y = bomb->position.y;

	// Up
	DRAW_BOMB_EX_TEXTURE(x, y - 1, EX_VERTICAL_TEXTURE);

	// Down
	DRAW_BOMB_EX_TEXTURE(x, y + 1, EX_VERTICAL_TEXTURE);

	// Left
	DRAW_BOMB_EX_TEXTURE(x - 1, y, EX_HORIZONTAL_TEXTURE);

	// Right
	DRAW_BOMB_EX_TEXTURE(x + 1, y, EX_HORIZONTAL_TEXTURE);
}

//game over if plater in explosion range
void player_in_bomb_range(struct bomb *bomb) {
	struct player_position player_pos = player_get_position();
	uint32_t player_grid_x = SCREEN_X_TO_GRID(player_pos.x + PLAYER_WIDTH / 2);
	uint32_t player_grid_y = SCREEN_Y_TO_GRID(player_pos.y + PLAYER_HEIGHT / 2);

	uint32_t bomb_x = bomb->position.x;
	uint32_t bomb_y = bomb->position.y;

	if (player_grid_x == bomb_x && player_grid_y == bomb_y) {
		game_set_game_state(GAME_STATE_GAMEOVER);
	}

	// Up
	if (player_grid_x == bomb_x && player_grid_y == bomb_y - 1) {
		game_set_game_state(GAME_STATE_GAMEOVER);
	}

	// Down
	if (player_grid_x == bomb_x && player_grid_y == bomb_y + 1) {
		game_set_game_state(GAME_STATE_GAMEOVER);
		
	}

	// Left
	if (player_grid_x == bomb_x - 1 && player_grid_y == bomb_y) {
		game_set_game_state(GAME_STATE_GAMEOVER);

	}

	// Right
	if (player_grid_x == bomb_x + 1 && player_grid_y == bomb_y) {
		game_set_game_state(GAME_STATE_GAMEOVER);
	}
}

static inline void bomb_draw(struct bomb bomb) {
	switch (bomb.state) {
		case BOMB_STATE_PRIMED:
			draw_texture(
				GRID_X_TO_SCREEN(bomb.position.x),
				GRID_Y_TO_SCREEN(bomb.position.y),
				BOMB_TEXTURE
			);
		break;
		case BOMB_STATE_EXPLODED:
			draw_texture(
				GRID_X_TO_SCREEN(bomb.position.x), 
				GRID_Y_TO_SCREEN(bomb.position.y), 
				EX_CENTER_TEXTURE
			);

			draw_valid_tile(&bomb);
			player_in_bomb_range(&bomb);
		break;
		default:
			puts("bomb_draw called with invalid bomb state");
			exit();
	}
}

void bombs_draw() {
	for (uint32_t i = 0; i < bombs_placed_count; ++i) {
		bomb_draw(bombs[i]);
	}
}

void bomb_place(uint32_t x, uint32_t y) {
	ASSERT1(x < GRID_WIDTH);
	ASSERT1(y < GRID_HEIGHT);

	if (bombs_placed_count == MAX_BOMB_COUNT) {
		return;
	} else if (bombs_placed_count > MAX_BOMB_COUNT) {
		puts("UHOH!!!");
		exit();
	}

	bombs[bombs_placed_count].position.x = x;
	bombs[bombs_placed_count].position.y = y;
	bombs[bombs_placed_count].state = BOMB_STATE_PRIMED;
	bombs[bombs_placed_count].time_left = BOMB_PRIMED_TIME_SHOWN;
	bombs_placed_count += 1;
}

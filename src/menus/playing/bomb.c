#include "menus/playing/bomb.h"

#include "utils/screen.h"
#include "assets/bomb_texture.h"

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
				return 1; // Should be removed
			}
		} break;
		default:
			puts("bomb_update called with invalid bomb state");
			exit();
	}

	return 0; // Should not be removed
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
			// draw bomb exploded
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

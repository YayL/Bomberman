#pragma once

#include <stdint.h>

#include "dtekv-lib.h"
#include "utils/timer.h"

// #define EMULATOR

#define TIME(TEXT, CODE) { \
    uint32_t start = timer_get_delta_us_no_reset(); \
    CODE; \
    uint32_t end = timer_get_delta_us_no_reset(); \
    puts(TEXT); \
    print_dec(end - start); \
    putc('\n'); \
}

#define ASSERTIONS_ENABLED
#ifdef ASSERTIONS_ENABLED

#define TO_STR(x) #x
#define TO_STR_VALUE(x) TO_STR(x)

#define ASSERT(EXPR, MSG) if (!(EXPR)) { puts("[" __FILE_NAME__ ":" TO_STR_VALUE(__LINE__) "] Assertion Failed: " MSG); }
#define ASSERT1(EXPR) ASSERT(EXPR, #EXPR)
#else
#define ASSERT(EXPR, MSG)
#define ASSERT1(EXPR)
#endif

struct position {
    uint32_t x, y;
};

static inline char hitbox_collision_check(
	uint32_t x1, uint32_t y1, uint32_t w1, uint32_t h1,
	uint32_t x2, uint32_t y2, uint32_t w2, uint32_t h2
) {
	return (
		x1 < x2 + w2 &&
		y1 < y2 + h2 &&
		x2 < x1 + w1 &&
		y2 < y1 + h1
	);
}

#include "utils/screen.h"
#include "menus/playing/map.h"

#define IS_AVAILABLE(X, Y) map_is_empty(SCREEN_X_TO_GRID(X), SCREEN_Y_TO_GRID(Y))
#define CLAMP(VALUE, MIN, MAX) if (VALUE < MIN) { VALUE = MIN; } else if (MAX < VALUE) { VALUE = MAX; }
static inline void entity_move(struct position * pos, uint32_t speed, int32_t dx, int32_t dy, uint32_t width, uint32_t height) {
	int32_t next_x = pos->x + dx * speed;
	int32_t next_y = pos->y + dy * speed;

	// This should never be a worry but just in case
	CLAMP(next_x, 0, SCREEN_WIDTH - width);
	CLAMP(next_y, 0, SCREEN_HEIGHT - height);

	// Since player is smaller than blocks we can just check the corners of the
	// player and see if they intersect with something

	// Unsure why the -1 are neccessary but they fixed an ¿issue?

	if (dx < 0) {
		const char top_left_dx = !IS_AVAILABLE(next_x, pos->y);
		const char bottom_left_dx = !IS_AVAILABLE(next_x, pos->y + height - 1);
		if (top_left_dx || bottom_left_dx) {
			// Going left and top left or bottom left intersects
			next_x = GRID_X_TO_SCREEN((SCREEN_X_TO_GRID(next_x) + 1));
		}
	} else if (dx > 0) {
		const char top_right_dx = !IS_AVAILABLE(next_x + width - 1, pos->y);
		const char bottom_right_dx = !IS_AVAILABLE(next_x + width - 1, pos->y + height - 1);
		if (top_right_dx || bottom_right_dx) {
			// Going right and top right or bottom right intersects
			next_x = GRID_X_TO_SCREEN(SCREEN_X_TO_GRID(next_x + width)) - width;
		}
	}


	if (dy < 0) {
		const char top_left_dy = !IS_AVAILABLE(pos->x, next_y);
		const char top_right_dy = !IS_AVAILABLE(pos->x + width - 1, next_y);
		if (top_left_dy || top_right_dy) {
			// Going up and top left or top right intersects
			next_y = GRID_Y_TO_SCREEN((SCREEN_Y_TO_GRID(next_y) + 1));
		}
	} else if (dy > 0) {
		const char bottom_left_dy = !IS_AVAILABLE(pos->x, next_y + height - 1);
		const char bottom_right_dy = !IS_AVAILABLE(pos->x + width - 1, next_y + height - 1);
		if (bottom_left_dy || bottom_right_dy) {
			// Going down and bottom left or bottom right intersects
			next_y = GRID_Y_TO_SCREEN(SCREEN_Y_TO_GRID(next_y + height)) - height;
		}
	}

	// Since we don't check dxdy it can start twiching in a block by going diagonally into it

	pos->x = next_x;
	pos->y = next_y;
}

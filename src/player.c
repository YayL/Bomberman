#include "player.h"
#include "common.h"
#include "utils/mem.h"
#include "utils/screen.h"
#include "utils/switches.h"

struct player player = {
	.speed = 1,
	.x = WORLD_X_TO_SCREEN(1),
	.y = WORLD_Y_TO_SCREEN(3)
};

#define CLAMP(VALUE, MIN, MAX) if (VALUE < MIN) { VALUE = MIN; } else if (MAX < VALUE) { VALUE = MAX; }
inline void player_move(int32_t dx, int32_t dy) {
	int32_t next_x = player.x + dx;
	int32_t next_y = player.y + dy;

	CLAMP(next_x, 0, SCREEN_WIDTH - PLAYER_WIDTH);
	CLAMP(next_y, 0, SCREEN_HEIGHT - PLAYER_HEIGHT);

	player.x = next_x;
	player.y = next_y;
}

#define CHECK_NTH_SWITCH(SWITCHES, N) ((SWITCHES >> N) & 0x1)
void player_update() {
	int32_t dx = 0, dy = 0;
	const uint32_t switches = switches_get_all_switch_states();

	// Right
	if (CHECK_NTH_SWITCH(switches, 0)) {
		dx += 1;
	}

	// Up
	if (CHECK_NTH_SWITCH(switches, 1)) {
		dy -= 1;
	}

	// Down
	if (CHECK_NTH_SWITCH(switches, 8)) {
		dy += 1;
	}

	// Left
	if (CHECK_NTH_SWITCH(switches, 9)) {
		dx -= 1;
	}

	player_move(dx, dy);
}

void mark_bomb_at_player_position(){
	int grid_x = SCREEN_X_TO_WORLD(player.x);
	int grid_y = SCREEN_Y_TO_WORLD(player.y);

	mark_bomb(grid_x, grid_y);
}

void player_draw() {
	draw_rectangle(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, RGB(7, 0, 0));
}

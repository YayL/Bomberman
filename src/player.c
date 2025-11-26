#include "player.h"
#include "utils/screen.h"
#include "dtekv-lib.h"
#include "utils/switches.h"

struct player player = {
	.speed = 1,
	.x = WORLD_X_TO_SCREEN(1),
	.y = WORLD_Y_TO_SCREEN(3)
};

void player_move_up() {
	player.y -= player.speed;
}

void player_update() {
	// uint32_t switch_states = switches_get_all_switch_states();
	// for (uint32_t i = 0; i <= 10; ++i) {
	// 	char switch_state = (switch_states >> i) & 0b1;
	// 	if (switch_state) {
	// 		puts("Switch ");
	// 		print_dec(i);
	// 		putc('\n');
	// 	}
	//
	// }

	if (switches_get_switch_state(0)){
		player_move_up();
	}
}

void player_draw() {
	// player.x = WORLD_X_TO_SCREEN(6), player.y = WORLD_Y_TO_SCREEN(7);

	uint32_t pixel = SCREEN_TO_PIXEL(player.x, player.y) + ((BLOCK_SIZE - PLAYER_WIDTH) / 2) + ((BLOCK_SIZE - PLAYER_HEIGHT) * SCREEN_WIDTH / 2);

	for (uint32_t i = 0; i < PLAYER_HEIGHT; ++i) {
		for (uint32_t j = 0; j < PLAYER_WIDTH; ++j) {
			FRAMEBUFFER_ADDRESS[pixel++] = RGB(7, 0, 0);
		}

		pixel += SCREEN_WIDTH - PLAYER_WIDTH;
	}
}

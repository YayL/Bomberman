#include "menus/playing/bomb.h"

#include "utils/screen.h"
#include "assets/bomb_texture.h"

static uint32_t bombs_placed_count = 0;
static struct bomb bombs[MAX_BOMB_COUNT] = {0};

static inline void bomb_update(uint32_t delta, struct bomb * bomb) {
	switch (bomb->state) {
		default:
			puts("bomb_update called with invalid bomb state");
			exit();
	}

}

void bombs_update(uint32_t delta) {
	for (uint32_t i = 0; i < bombs_placed_count; ++i) {
		bomb_update(delta, &bombs[i]);
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
		default:
			puts("bomb_draw called with invalid bomb state");
			exit();
	}
}

void bombs_draw() {
	puts("Bombs: ");
	print_dec(bombs_placed_count);
	putc('\n');

	puts("Address: ");
	print_hex32((uint32_t) &bombs_placed_count);
	putc('\n');

	for (uint32_t i = 0; i < bombs_placed_count; ++i) {
		bomb_draw(bombs[i]);
	}
}

void bomb_place(uint32_t x, uint32_t y) {
	ASSERT1(x < GRID_WIDTH);
	ASSERT1(y < GRID_HEIGHT);

	puts("Bombs: ");
	print_dec(bombs_placed_count);
	putc('\n');

	if (bombs_placed_count == MAX_BOMB_COUNT) {
		return;
	} else if (bombs_placed_count > MAX_BOMB_COUNT) {
		puts("UHOH!!!");
		exit();
	}

	bombs[bombs_placed_count].position.x = x;
	bombs[bombs_placed_count].position.y = y;
	bombs[bombs_placed_count].state = BOMB_STATE_PRIMED;
	bombs_placed_count += 1;
}

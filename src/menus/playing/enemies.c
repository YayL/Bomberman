/*
*	Contributors: Zimon
*/

#include "menus/playing/enemies.h"

#include "menus/playing/player.h"

#include "utils/screen.h"
#include "utils/random.h"
#include "assets/monster_texture.h"

#include "game.h"

static uint32_t enemies_alive_count = 0;
static struct enemy enemies[MAX_ENEMY_COUNT] = {0};

uint32_t enemies_get_alive_count() {
	return enemies_alive_count;
}

void enemies_add(uint32_t x, uint32_t y) {
	if (enemies_alive_count >= MAX_ENEMY_COUNT) {
		return;
	}

	enemies[enemies_alive_count++] = (struct enemy) { 
		.position = { .x = GRID_X_TO_SCREEN(x), .y = GRID_Y_TO_SCREEN(y) },
		.speed = ENEMY_SPEED
	};
}

void enemies_clear() {
	enemies_alive_count = 0;
}

void enemy_remove(uint32_t index) {
	ASSERT1(index < MAX_ENEMY_COUNT);

	// Shift down enemies
	for (uint32_t i = index + 1; i < MAX_ENEMY_COUNT; ++i) {
		enemies[i - 1] = enemies[i];
	}

	enemies_alive_count -= 1;
}

char enemy_update(struct enemy * enemy, uint32_t delta) {
	if (enemy->killed) {
		return 1;
	}

	enemy->timer -= delta;
	if (enemy->timer > ENEMY_DIRECTION_CHANGE_TIMER) {
		enemy->timer = ENEMY_DIRECTION_CHANGE_TIMER;
		switch (random_get_in_range(0, 4)) {
			case 0: enemy->movement = (struct movement) {.dx = -1, .dy = 0}; break;
			case 1: enemy->movement = (struct movement) {.dx = 0, .dy = 1}; break;
			case 2: enemy->movement = (struct movement) {.dx = 1, .dy = 0}; break;
			case 3: enemy->movement = (struct movement) {.dx = 0, .dy = -1}; break;
		}
	}

	entity_move(&enemy->position, enemy->speed, enemy->movement.dx, enemy->movement.dy, ENEMY_WIDTH, ENEMY_HEIGHT);

	struct position p_pos = player_get_position();
	struct position e_pos = { .x = enemy->position.x, .y = enemy->position.y };

	if (hitbox_collision_check(p_pos.x, p_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT, e_pos.x, e_pos.y, ENEMY_WIDTH, ENEMY_HEIGHT)) {
		player_kill();
	}

	return 0;
}

void enemies_update(uint32_t delta) {
	// Yes this is super ugly; deal with it
	for (uint32_t i = 0; i < enemies_alive_count;) {
		if (enemy_update(&enemies[i], delta)) {
			enemy_remove(i);
		} else {
			i += 1;
		}
	}

	if (enemies_alive_count == 0) {
		game_set_game_state(GAME_STATE_GAMEOVER);
	}
}

void enemy_draw(struct enemy enemy) {
	draw_texture(
		enemy.position.x,
		enemy.position.y,
		MONSTER_TEXTURE
	);
}

void enemies_draw() {
	for (uint32_t i = 0; i < enemies_alive_count; ++i) {
		enemy_draw(enemies[i]);
	}
}

static inline void enemy_check_explosion_collision(uint32_t bx, uint32_t by, struct enemy * enemy) {
	struct position e_pos = enemy->position;
	const char vertical_explosion_collision = hitbox_collision_check(
													e_pos.x, e_pos.y, ENEMY_WIDTH, ENEMY_HEIGHT,
													GRID_X_TO_SCREEN(bx), GRID_Y_TO_SCREEN(by - 1), BLOCK_SIZE, BLOCK_SIZE * 3
												);
	const char horizontal_explosion_collision = hitbox_collision_check(
													e_pos.x, e_pos.y, ENEMY_WIDTH, ENEMY_HEIGHT,
													GRID_X_TO_SCREEN(bx - 1), GRID_Y_TO_SCREEN(by), BLOCK_SIZE * 3, BLOCK_SIZE
												);

	if (vertical_explosion_collision || horizontal_explosion_collision) {
		enemy->killed = 1;
	}
}

void enemies_check_explosion_collision(uint32_t bx, uint32_t by) {
	for (uint32_t i = 0; i < enemies_alive_count; ++i) {
		enemy_check_explosion_collision(bx, by, &enemies[i]);
	}
}

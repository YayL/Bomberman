#pragma once

enum game_state {
	GAME_STATE_NONE,
	GAME_STATE_START,
	GAME_STATE_PLAYING
};

void game_set_is_running(char state);
void game_set_game_state(enum game_state state);

void game_init();
void game_run();

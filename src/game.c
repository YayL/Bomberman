#include "game.h"
#include "dtekv-lib.h"

#include "assets/test.txt.h"

char is_running = 1;

void game_set_running_state(char state) {
	is_running = state;
}

void game_init() {

}

void game_run() {
	while (is_running) {

	}
}

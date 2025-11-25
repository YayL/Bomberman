#include "game.h"
#include "dtekv-lib.h"

#include "assets/test.txt.h"
#include <stdint.h>
#include <stddef.h>

#define FRAMEBUFFER_ADDRESS ((volatile char*)0x08000000)

static char is_running = 1;

void game_set_running_state(char state) {
	is_running = state;
}

void fill_background(int color) {
    for (int i = 0; i < 320*240; i++)
        FRAMEBUFFER_ADDRESS[i] = color; 
}

void game_init() {
	fill_background(80);

}

void game_run() {
	while (is_running) {

	}
}

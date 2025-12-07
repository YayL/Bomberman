#include "dtekv-lib.h"
#include "utils/timer.h"
#include "game.h"

#include "utils/hw_counters.h"

int main() {
	timer_init();
	timer_enable_interrupt();

	counters_clear();

	game_init();
	game_run();

	return 0;
}

void handle_interrupt(void) {
	timer_ack();
}

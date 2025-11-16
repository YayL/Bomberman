#include "dtekv-lib.h"
#include "utils/timer.h"
#include "game.h"

void print_delta_time() {
	uint32_t delta_time = timer_get_delta();

	puts("Delta: ");
	print_dec(delta_time);
	puts("\n");
}

int main() {
	timer_init();
	timer_enable_interrupt();

	game_init();
	game_run();

	return 0;
}


void handle_interrupt(void) {
	timer_ack();
	puts("Interrupt");
}

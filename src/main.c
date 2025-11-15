#include "dtekv-lib.h"
#include "utils/timer.h"

void print_delta_time() {
	uint32_t delta_time = timer_get_delta();

	puts("Delta: ");
	print_dec(delta_time);
	puts("\n");
}

uint32_t fib(uint32_t n) {
	if (n < 2) {
		return 1;
	}

	uint32_t i, curr = i = 2;
	uint32_t prev = 1;
	uint32_t temp;

	while (i++ < n) {
		temp = curr;
		curr += prev;
		prev = temp;
	}

	return curr;
}

int main() {
	timer_init();
	timer_enable_interrupt();

	timer_get_delta();
	puts("Hello, World!\n");
	print_delta_time();

	for (uint32_t i = 0; i < 30; ++i) {
		volatile unsigned int a = fib(i);
		print_delta_time();
	}

	return 0;
}


void handle_interrupt(void) {
	timer_ack();
	puts("Interrupt");
}

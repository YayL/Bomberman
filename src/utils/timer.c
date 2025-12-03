#include "utils/timer.h"

#define TIMER_ADDRESS 0x04000020

volatile struct timer {
	struct timer_status {
		uint32_t TO: 1;
		const uint32_t RUN: 1;
	} status;

	struct timer_control {
		uint32_t ITO: 1;
		uint32_t CONT: 1;
		uint32_t START: 1;
		uint32_t STOP: 1;
	} control;

	struct timer_period {
		uint32_t low;
		uint32_t high;
	} period;

	struct timer_snap {
		uint32_t low;
		uint32_t high;
	} snap;
} * timer = (void *) TIMER_ADDRESS;

uint32_t last_snapshot = 0;

#define TIMER_FREQ 3e7 // 30 MHz
#define TIMER_PERIOD UINT32_MAX  // Maximum timer period allowed and handles timer_get_delta wrap around automatically

static inline void timer_set_period(uint32_t period) {
#ifndef EMULATOR
	timer->period.low = period & 0xffff;
	timer->period.high = period >> 16;
#endif
}

volatile static inline uint32_t timer_get_snap() {
#ifndef EMULATOR
	timer->snap.low = 0; // request snapshot
	return (timer->snap.low & 0xffff) | (timer->snap.high << 16);
#else
	return 15000; // 15ms
#endif
}

void timer_enable_interrupt() {
#ifndef EMULATOR
	timer->control.ITO = 1;

	asm volatile(
		"csrsi mstatus, 3;"
		"csrsi mie, 16;"
	);
#endif
}

void timer_init() {
#ifndef EMULATOR
	timer_set_period(UINT32_MAX);
	last_snapshot = UINT32_MAX;
	timer->control.CONT = 1;
	timer->control.START = 1;
	timer->status.TO = 1;
#endif
}

void timer_ack() {
#ifndef EMULATOR
	timer->status.TO = 1;
#endif
}

uint32_t timer_get_delta_us_no_reset() {
#ifndef EMULATOR
	uint32_t current = timer_get_snap();
	uint32_t elapsed = last_snapshot - current; // Fix wrap around if TIMER_PERIOD is modified from UINT32_MAX

	return elapsed / (TIMER_FREQ / 1e6);
#else
	return 15000;
#endif
}

uint32_t timer_get_delta_us() {
#ifndef EMULATOR
	uint32_t current = timer_get_snap();
	uint32_t elapsed = last_snapshot - current; // Fix wrap around if TIMER_PERIOD is modified from UINT32_MAX
	last_snapshot = current;

	return elapsed / (TIMER_FREQ / 1e6);
#else
	return 15000;
#endif
}

#ifndef EMULATOR
void timer_wait(uint32_t wait_time) {
	timer_get_delta_us();
	while (timer_get_delta_us_no_reset() < wait_time);
}
#endif

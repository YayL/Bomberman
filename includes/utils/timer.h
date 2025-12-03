#pragma once

#include "common.h"
void timer_enable_interrupt();

void timer_init();

void timer_ack();

uint32_t timer_get_delta_us_no_reset();
uint32_t timer_get_delta_us();

#ifndef EMULATOR
void timer_wait(uint32_t wait_time);
#endif

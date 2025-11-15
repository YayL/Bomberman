#pragma once

#include <stdint.h>

void timer_enable_interrupt();

void timer_init();

void timer_ack();

uint32_t timer_get_delta();

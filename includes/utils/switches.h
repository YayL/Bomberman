#pragma once
#include <stdint.h>

#define CHECK_NTH_SWITCH(SWITCHES, N) ((SWITCHES >> N) & 0x1)

uint32_t switches_get_switch_state(uint32_t switches);
uint32_t switches_get_all_switch_states();

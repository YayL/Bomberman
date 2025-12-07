#pragma once

#include <stdint.h>

void random_seed(uint32_t seed);

uint32_t random_get_in_range(uint32_t min, uint32_t max);

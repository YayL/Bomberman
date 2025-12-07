/*
	Contributor: Axel
*/

#include "utils/random.h"

#define IN_RANGE(VALUE, MIN, MAX) (((VALUE) % (MAX - MIN + 1)) + MIN)

static uint32_t seed = 0xdeadbeef;

void random_seed(uint32_t new_seed) {
	seed = new_seed;
}

uint32_t fast_rand() {
	seed ^= seed << 13;
	seed ^= seed >> 17;
	seed ^= seed << 5;

	return seed;
}

uint32_t random_get_in_range(uint32_t min, uint32_t max) {
	uint32_t rand = fast_rand();
	return IN_RANGE(rand, min, max);
}

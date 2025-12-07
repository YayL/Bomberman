#pragma once

/*
*	Contributors: Zimon
*/

#include <stdint.h>
#include "common.h"
#include "utils/math.h"

#define COUNTERS_LIST(f) \
	f(mcycle) \
	f(minstret) \
	f(mhpmcounter3 ) \
	f(mhpmcounter4) \
	f(mhpmcounter5) \
	f(mhpmcounter6) \
	f(mhpmcounter7) \
	f(mhpmcounter8) \
	f(mhpmcounter9)

#define COUNTERS_STRUCT_EL(NAME) uint32_t NAME;
struct hw_counters {
	COUNTERS_LIST(COUNTERS_STRUCT_EL)
};

#define COUNTERS_CLEAR_FUNC_NAME(NAME) NAME##_clear
#define COUNTERS_CLEAR_FUNC(NAME) static inline void COUNTERS_CLEAR_FUNC_NAME(NAME)() { asm volatile ("csrw " #NAME ", x0"); }

COUNTERS_LIST(COUNTERS_CLEAR_FUNC);

#define COUNTERS_GET_FUNC_NAME(NAME) NAME##_get
#define COUNTERS_GET_FUNC(NAME) static inline uint32_t COUNTERS_GET_FUNC_NAME(NAME)() { uint32_t ret; asm volatile ("csrr %0, " #NAME : "=r"(ret)); return ret; }

COUNTERS_LIST(COUNTERS_GET_FUNC);

#define COUNTERS_CALL_CLEAR(NAME) COUNTERS_CLEAR_FUNC_NAME(NAME)();
static inline void counters_clear() {
#ifndef EMULATOR
	COUNTERS_LIST(COUNTERS_CALL_CLEAR);
#endif
}

#define COUNTERS_POP_STRUCT(NAME) counters->NAME += COUNTERS_GET_FUNC_NAME(NAME)();
static inline void counters_popualte(struct hw_counters * counters) {
#ifndef EMULATOR
	COUNTERS_LIST(COUNTERS_POP_STRUCT);
#endif
}

#define PRINT_DEC(TEXT, VALUE) \
	puts(TEXT); \
	print_dec((VALUE)); \
	putc('\n');

#define PRINT_HW_COUNTER(NAME) PRINT_DEC(#NAME ": \t", counters->NAME);

static inline void counter_report(struct hw_counters * counters) {
#ifndef EMULATOR
	puts("HW Counter Report:\n");
	puts("==================\n");
	COUNTERS_LIST(PRINT_HW_COUNTER);
	puts("==================\n");
	puts("Metrics:\n");
	puts("==================\n");
	PRINT_DEC("Execution Time:\t", __div64_32(counters->mcycle, 30000000 / 1000));
	PRINT_DEC("IPC(%):\t\t", __div64_32(10000 * (uint64_t) counters->minstret, counters->mcycle));
	PRINT_DEC("I-cache miss:\t", __div64_32(10000 * ((uint64_t) counters->minstret - counters->mhpmcounter4), counters->minstret));
	PRINT_DEC("D-cache miss:\t", __div64_32(10000 * ((uint64_t) counters->mhpmcounter3 - counters->mhpmcounter5), counters->mhpmcounter3));
	PRINT_DEC("DH stalls:\t", __div64_32(10000 * (uint64_t) counters->mhpmcounter8, counters->mcycle));
	PRINT_DEC("Mem Intensity(%):", __div64_32(10000 * (uint64_t) counters->mhpmcounter3, counters->minstret));
	puts("==================\n");
#else
	puts("HW Counters do not work on emulator");
#endif
}

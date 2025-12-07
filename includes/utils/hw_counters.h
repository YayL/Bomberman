#pragma once
#include <stdint.h>
#include "dtekv-lib.h"
#include "utils/math.h"
#include "utils/mem.h"

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
	COUNTERS_LIST(COUNTERS_CALL_CLEAR);
}

#define COUNTERS_POP_STRUCT(NAME) counters->NAME += COUNTERS_GET_FUNC_NAME(NAME)();
static inline void counters_get(struct hw_counters * counters) {
	COUNTERS_LIST(COUNTERS_POP_STRUCT);
}

#define PRINT_DEC(TEXT, VALUE) \
	puts(TEXT); \
	print_dec((VALUE)); \
	putc('\n');

#define PRINT_HW_COUNTER(NAME) PRINT_DEC(#NAME ": \t", counters->NAME);

static inline void counter_report(struct hw_counters * counters) {
	puts("HW Counter Report:\n");
	puts("==================\n");
	COUNTERS_LIST(PRINT_HW_COUNTER);
	puts("==================\n");
	puts("Metrics:\n");
	puts("==================\n");
	PRINT_DEC("Execution Time: \t", __div64_32(counters->mcycle, 30000000));
	PRINT_DEC("IPC(%):\t\t\t", __div64_32(100 * (uint64_t) counters->minstret, counters->mcycle));
	PRINT_DEC("D-cache miss(‰)\t\t", __div64_32(1000 * counters->mhpmcounter5, counters->mhpmcounter3));
	PRINT_DEC("I-cache miss(‰)\t\t", __div64_32(1000 * counters->mhpmcounter4, counters->minstret));
	PRINT_DEC("Mem Intensity(%):\t", __div64_32(100 * counters->mhpmcounter3, counters->minstret));
	PRINT_DEC("Cache misses:\t\t", counters->mhpmcounter4 + counters->mhpmcounter5);
	puts("==================\n");
}

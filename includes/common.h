#pragma once

#include <stdint.h>

#include "dtekv-lib.h"
#include "utils/timer.h"

#define EMULATOR

#define TIME(TEXT, CODE) { \
    uint32_t start = timer_get_delta_us_no_reset(); \
    CODE; \
    uint32_t end = timer_get_delta_us_no_reset(); \
    puts(TEXT); \
    print_dec(end - start); \
    putc('\n'); \
}

#define ASSERTIONS_ENABLED
#ifdef ASSERTIONS_ENABLED

#define TO_STR(x) #x
#define TO_STR_VALUE(x) TO_STR(x)

#define ASSERT(EXPR, MSG) if (!(EXPR)) { puts("[" __FILE_NAME__ ":" TO_STR_VALUE(__LINE__) "] Assertion Failed: " MSG); }
#define ASSERT1(EXPR) ASSERT(EXPR, #EXPR)
#else
#define ASSERT(EXPR, MSG)
#define ASSERT1(EXPR)
#endif

#pragma once

#include "dtekv-lib.h"
#include "utils/timer.h"

#define TIME(TEXT, CODE) { \
    uint32_t start = timer_get_delta_us_no_reset(); \
    CODE; \
    uint32_t end = timer_get_delta_us_no_reset(); \
    puts(TEXT); \
    print_dec(end - start); \
    putc('\n'); \
}

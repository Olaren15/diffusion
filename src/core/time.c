#include "core/time.h"

#define NANOSECONDS_PER_SECOND 1000000

#ifdef _WIN32

#include "platform/win32/win32_types.h"

static LARGE_INTEGER ticks_per_second;

uint64_t get_elapsed_nanoseconds() {
    if (!ticks_per_second.QuadPart) {
        QueryPerformanceFrequency(&ticks_per_second);
    }

    LARGE_INTEGER current_tick;
    QueryPerformanceCounter(&current_tick);

    return current_tick.QuadPart * NANOSECONDS_PER_SECOND / ticks_per_second.QuadPart;
}

double nanoseconds_to_seconds(uint64_t nanoseconds) {
    return (double)nanoseconds / NANOSECONDS_PER_SECOND;
}

#endif // _WIN32

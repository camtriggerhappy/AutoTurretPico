#include "clockFix.h"

// micro-ROS general includes with general functionality.
#include <pico/types.h>
#include <pico/time.h>

int clock_gettime(clockid_t unused, struct timespec *tp)
{
    uint64_t m = time_us_64();
    tp->tv_sec = m / 1000000;
    tp->tv_nsec = (m % 1000000) * 1000;
    return 0;
}

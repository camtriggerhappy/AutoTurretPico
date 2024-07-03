#include "clockFix.h"

// micro-ROS general includes with general functionality.
#include <pico/types.h>
#include <pico/time.h>


int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
  (void) clock_id;

  uint64_t microseconds_elapsed = to_us_since_boot(get_absolute_time());

  // Handle here possible rollovers of your platform timers if required.

  tp->tv_sec = microseconds_elapsed / (10^6);
  tp->tv_nsec = (microseconds_elapsed % (10^6)) * (10^3);

  return 0;
}